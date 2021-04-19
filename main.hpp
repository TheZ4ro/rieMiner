// (c) 2017-2021 Pttn and contributors (https://github.com/Pttn/rieMiner)

#ifndef HEADER_main_hpp
#define HEADER_main_hpp

#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>
#include "tools.hpp"

#define versionString	"rieMiner 0.92d'"
#define primeTableFile	"PrimeTable64.bin"

extern int DEBUG;
extern std::string confPath;

#define DBG(x) if (DEBUG) {x;};
#define DBG_VERIFY(x) if (DEBUG > 1) { x; };
#define ERRORMSG(message) std::cerr << __func__ << ": " << message << " :| !" << std::endl

static const std::vector<std::pair<std::vector<uint64_t>, std::vector<uint64_t>>> defaultConstellationData = {
	// 1-tuples
	{{0}, {380284918609481ULL, 437163765888581ULL, 701889794782061ULL, 980125031081081ULL, 1277156391416021ULL, 1487854607298791ULL, 1833994713165731ULL, 2115067287743141ULL, 2325810733931801ULL, 3056805353932061ULL, 3252606350489381ULL, 3360877662097841ULL, 3501482688249431ULL, 3595802556731501ULL, 3843547642594391ULL, 5000014653723821ULL, 5861268883004651ULL, 7486645325734691ULL, 7933248530182091ULL, 8760935349271991ULL, 8816939536219931ULL, 8871465225933041ULL, 9354490866900911ULL, 11096062107205961ULL, 11096434547374061ULL, 13764730155211151ULL, 13884748604026031ULL, 17438667992681051ULL, 20362378935668501ULL, 20471700514990841ULL, 20475715985020181ULL, 20614750499829371ULL, 21465425387541251ULL, 21628360938574121ULL, 21817283854511261ULL, 22238558064758921ULL, 22318056296221571ULL, 22733842556089781ULL, 22849881428489231ULL, 23382987892499351ULL, 23417442472403711ULL, 25964083184094941ULL, 26515897161980111ULL, 29512383574028471ULL, 30074756036270831ULL, 30310618347929651ULL, 30402250951007051ULL, 30413977411117031ULL, 33502273017038711ULL, 33508988966488151ULL, 33976718302847051ULL, 34783522781262371ULL, 37564605737538611ULL, 37606024583356961ULL, 39138758504100371ULL, 40205947750578341ULL, 40257009922154141ULL, 40392614725338761ULL, 40504121267225501ULL, 41099072498143391ULL, 41289201480321911ULL, 41543933848913381ULL, 42218492028808211ULL, 43938526447515431ULL}}, // OEIS A213645 (12-tuples)
	// 2-tuples
	{{0, 2}, {380284918609481ULL, 437163765888581ULL, 701889794782061ULL, 980125031081081ULL, 1277156391416021ULL, 1487854607298791ULL, 1833994713165731ULL, 2115067287743141ULL, 2325810733931801ULL, 3056805353932061ULL, 3252606350489381ULL, 3360877662097841ULL, 3501482688249431ULL, 3595802556731501ULL, 3843547642594391ULL, 5000014653723821ULL, 5861268883004651ULL, 7486645325734691ULL, 7933248530182091ULL, 8760935349271991ULL, 8816939536219931ULL, 8871465225933041ULL, 9354490866900911ULL, 11096062107205961ULL, 11096434547374061ULL, 13764730155211151ULL, 13884748604026031ULL, 17438667992681051ULL, 20362378935668501ULL, 20471700514990841ULL, 20475715985020181ULL, 20614750499829371ULL, 21465425387541251ULL, 21628360938574121ULL, 21817283854511261ULL, 22238558064758921ULL, 22318056296221571ULL, 22733842556089781ULL, 22849881428489231ULL, 23382987892499351ULL, 23417442472403711ULL, 25964083184094941ULL, 26515897161980111ULL, 29512383574028471ULL, 30074756036270831ULL, 30310618347929651ULL, 30402250951007051ULL, 30413977411117031ULL, 33502273017038711ULL, 33508988966488151ULL, 33976718302847051ULL, 34783522781262371ULL, 37564605737538611ULL, 37606024583356961ULL, 39138758504100371ULL, 40205947750578341ULL, 40257009922154141ULL, 40392614725338761ULL, 40504121267225501ULL, 41099072498143391ULL, 41289201480321911ULL, 41543933848913381ULL, 42218492028808211ULL, 43938526447515431ULL}}, // OEIS A213645 (12-tuples)
	// 3-tuples
	{{0, 2, 4}, {380284918609481ULL, 437163765888581ULL, 701889794782061ULL, 980125031081081ULL, 1277156391416021ULL, 1487854607298791ULL, 1833994713165731ULL, 2115067287743141ULL, 2325810733931801ULL, 3056805353932061ULL, 3252606350489381ULL, 3360877662097841ULL, 3501482688249431ULL, 3595802556731501ULL, 3843547642594391ULL, 5000014653723821ULL, 5861268883004651ULL, 7486645325734691ULL, 7933248530182091ULL, 8760935349271991ULL, 8816939536219931ULL, 8871465225933041ULL, 9354490866900911ULL, 11096062107205961ULL, 11096434547374061ULL, 13764730155211151ULL, 13884748604026031ULL, 17438667992681051ULL, 20362378935668501ULL, 20471700514990841ULL, 20475715985020181ULL, 20614750499829371ULL, 21465425387541251ULL, 21628360938574121ULL, 21817283854511261ULL, 22238558064758921ULL, 22318056296221571ULL, 22733842556089781ULL, 22849881428489231ULL, 23382987892499351ULL, 23417442472403711ULL, 25964083184094941ULL, 26515897161980111ULL, 29512383574028471ULL, 30074756036270831ULL, 30310618347929651ULL, 30402250951007051ULL, 30413977411117031ULL, 33502273017038711ULL, 33508988966488151ULL, 33976718302847051ULL, 34783522781262371ULL, 37564605737538611ULL, 37606024583356961ULL, 39138758504100371ULL, 40205947750578341ULL, 40257009922154141ULL, 40392614725338761ULL, 40504121267225501ULL, 41099072498143391ULL, 41289201480321911ULL, 41543933848913381ULL, 42218492028808211ULL, 43938526447515431ULL}}, // OEIS A213645 (12-tuples)
	{{0, 4, 2}, {1418575498573ULL, 27899359258003ULL, 34460918582323ULL, 76075560855373ULL, 186460616596333ULL, 218021188549243ULL, 234280497145543ULL, 282854319391723ULL, 345120905374093ULL, 346117552180633ULL, 604439135284063ULL, 727417501795063ULL, 1041814617748753ULL, 1090754719898923ULL, 1539765965257753ULL, 3152045700948223ULL, 3323127757029313ULL, 3449427485143873ULL, 4422879865247923ULL, 4525595253334003ULL, 4730773080017833ULL, 5462875671033013ULL, 6147764065076713ULL, 6205707895751443ULL, 6308411019731053ULL, 7582919852522863ULL, 7791180222409663ULL, 9162887985581563ULL, 9305359177794913ULL, 10096106139749863ULL, 10349085616714693ULL, 10744789916260633ULL, 10932016019429353ULL, 11140102475962693ULL, 12448240792011103ULL, 14727257011031413ULL, 16892267700442213ULL, 17963729763800053ULL, 18908121647739403ULL, 19028992697498863ULL, 19756696515786463ULL, 20252223877980943ULL, 20429666791949263ULL, 21680774776901473ULL, 21682173462980263ULL, 23076668788453513ULL, 24036602580170413ULL, 24101684579532793ULL, 25053289894907353ULL, 25309078073142943ULL, 25662701041982083ULL, 25777719656829373ULL, 26056424604564433ULL, 26315911419972253ULL, 26866456999592443ULL, 26887571851660753ULL, 27303559129791793ULL, 27839080743588193ULL, 28595465291933773ULL, 29137316070747733ULL, 30824439453812083ULL, 31395828815154883ULL, 31979851757518513ULL, 32897714831936803ULL}}, // OEIS A213601 (12-tuples) + 6
	// 4-tuples
	{{0, 2, 4, 2}, {380284918609481ULL, 437163765888581ULL, 701889794782061ULL, 980125031081081ULL, 1277156391416021ULL, 1487854607298791ULL, 1833994713165731ULL, 2115067287743141ULL, 2325810733931801ULL, 3056805353932061ULL, 3252606350489381ULL, 3360877662097841ULL, 3501482688249431ULL, 3595802556731501ULL, 3843547642594391ULL, 5000014653723821ULL, 5861268883004651ULL, 7486645325734691ULL, 7933248530182091ULL, 8760935349271991ULL, 8816939536219931ULL, 8871465225933041ULL, 9354490866900911ULL, 11096062107205961ULL, 11096434547374061ULL, 13764730155211151ULL, 13884748604026031ULL, 17438667992681051ULL, 20362378935668501ULL, 20471700514990841ULL, 20475715985020181ULL, 20614750499829371ULL, 21465425387541251ULL, 21628360938574121ULL, 21817283854511261ULL, 22238558064758921ULL, 22318056296221571ULL, 22733842556089781ULL, 22849881428489231ULL, 23382987892499351ULL, 23417442472403711ULL, 25964083184094941ULL, 26515897161980111ULL, 29512383574028471ULL, 30074756036270831ULL, 30310618347929651ULL, 30402250951007051ULL, 30413977411117031ULL, 33502273017038711ULL, 33508988966488151ULL, 33976718302847051ULL, 34783522781262371ULL, 37564605737538611ULL, 37606024583356961ULL, 39138758504100371ULL, 40205947750578341ULL, 40257009922154141ULL, 40392614725338761ULL, 40504121267225501ULL, 41099072498143391ULL, 41289201480321911ULL, 41543933848913381ULL, 42218492028808211ULL, 43938526447515431ULL}}, // OEIS A213645
	// 5-tuples
	{{0, 2, 4, 2, 4}, {380284918609481ULL, 437163765888581ULL, 701889794782061ULL, 980125031081081ULL, 1277156391416021ULL, 1487854607298791ULL, 1833994713165731ULL, 2115067287743141ULL, 2325810733931801ULL, 3056805353932061ULL, 3252606350489381ULL, 3360877662097841ULL, 3501482688249431ULL, 3595802556731501ULL, 3843547642594391ULL, 5000014653723821ULL, 5861268883004651ULL, 7486645325734691ULL, 7933248530182091ULL, 8760935349271991ULL, 8816939536219931ULL, 8871465225933041ULL, 9354490866900911ULL, 11096062107205961ULL, 11096434547374061ULL, 13764730155211151ULL, 13884748604026031ULL, 17438667992681051ULL, 20362378935668501ULL, 20471700514990841ULL, 20475715985020181ULL, 20614750499829371ULL, 21465425387541251ULL, 21628360938574121ULL, 21817283854511261ULL, 22238558064758921ULL, 22318056296221571ULL, 22733842556089781ULL, 22849881428489231ULL, 23382987892499351ULL, 23417442472403711ULL, 25964083184094941ULL, 26515897161980111ULL, 29512383574028471ULL, 30074756036270831ULL, 30310618347929651ULL, 30402250951007051ULL, 30413977411117031ULL, 33502273017038711ULL, 33508988966488151ULL, 33976718302847051ULL, 34783522781262371ULL, 37564605737538611ULL, 37606024583356961ULL, 39138758504100371ULL, 40205947750578341ULL, 40257009922154141ULL, 40392614725338761ULL, 40504121267225501ULL, 41099072498143391ULL, 41289201480321911ULL, 41543933848913381ULL, 42218492028808211ULL, 43938526447515431ULL}}, // OEIS A213645 (12-tuples)
	{{0, 4, 2, 4, 2}, {1418575498597ULL, 27899359258027ULL, 34460918582347ULL, 76075560855397ULL, 186460616596357ULL, 218021188549267ULL, 234280497145567ULL, 282854319391747ULL, 345120905374117ULL, 346117552180657ULL, 604439135284087ULL, 727417501795087ULL, 1041814617748777ULL, 1090754719898947ULL, 1539765965257777ULL, 3152045700948247ULL, 3323127757029337ULL, 3449427485143897ULL, 4422879865247947ULL, 4525595253334027ULL, 4730773080017857ULL, 5462875671033037ULL, 6147764065076737ULL, 6205707895751467ULL, 6308411019731077ULL, 7582919852522887ULL, 7791180222409687ULL, 9162887985581587ULL, 9305359177794937ULL, 10096106139749887ULL, 10349085616714717ULL, 10744789916260657ULL, 10932016019429377ULL, 11140102475962717ULL, 12448240792011127ULL, 14727257011031437ULL, 16892267700442237ULL, 17963729763800077ULL, 18908121647739427ULL, 19028992697498887ULL, 19756696515786487ULL, 20252223877980967ULL, 20429666791949287ULL, 21680774776901497ULL, 21682173462980287ULL, 23076668788453537ULL, 24036602580170437ULL, 24101684579532817ULL, 25053289894907377ULL, 25309078073142967ULL, 25662701041982107ULL, 25777719656829397ULL, 26056424604564457ULL, 26315911419972277ULL, 26866456999592467ULL, 26887571851660777ULL, 27303559129791817ULL, 27839080743588217ULL, 28595465291933797ULL, 29137316070747757ULL, 30824439453812107ULL, 31395828815154907ULL, 31979851757518537ULL, 32897714831936827ULL}}, // OEIS A213601 (12-tuples) + 30
	// 6-tuples
	{{0, 4, 2, 4, 2, 4}, {1091257ULL, 1615837ULL, 1954357ULL, 2822707ULL, 2839927ULL, 3243337ULL, 3400207ULL, 6005887ULL, 6503587ULL, 7187767ULL, 7641367ULL, 8061997ULL, 8741137ULL, 10526557ULL, 11086837ULL, 11664547ULL, 14520547ULL, 14812867ULL, 14834707ULL, 14856757ULL, 16025827ULL, 16094707ULL, 18916477ULL, 19197247ULL, 19634047ULL, 19800367ULL, 20112217ULL, 20247037ULL, 21321187ULL, 21850177ULL, 22587277ULL, 24786397ULL, 25009417ULL, 25524127ULL, 27305557ULL, 29153557ULL, 31563937ULL, 31875577ULL, 32836747ULL, 33575947ULL, 36319177ULL, 36985297ULL, 37055647ULL, 40660717ULL, 41214067ULL, 41763427ULL, 41927437ULL, 44842867ULL, 45974557ULL, 47204737ULL, 48660247ULL, 49157737ULL, 50685907ULL, 50943787ULL, 51255637ULL, 53204857ULL, 53266387ULL, 55057897ULL, 56431927ULL, 57812467ULL, 59877397ULL, 61052347ULL, 62757967ULL, 63655717ULL}}, // OEIS A022013
	// 7-tuples
	{{0, 2, 4, 2, 4, 6, 2}, {380284918609481ULL, 437163765888581ULL, 701889794782061ULL, 980125031081081ULL, 1277156391416021ULL, 1487854607298791ULL, 1833994713165731ULL, 2115067287743141ULL, 2325810733931801ULL, 3056805353932061ULL, 3252606350489381ULL, 3360877662097841ULL, 3501482688249431ULL, 3595802556731501ULL, 3843547642594391ULL, 5000014653723821ULL, 5861268883004651ULL, 7486645325734691ULL, 7933248530182091ULL, 8760935349271991ULL, 8816939536219931ULL, 8871465225933041ULL, 9354490866900911ULL, 11096062107205961ULL, 11096434547374061ULL, 13764730155211151ULL, 13884748604026031ULL, 17438667992681051ULL, 20362378935668501ULL, 20471700514990841ULL, 20475715985020181ULL, 20614750499829371ULL, 21465425387541251ULL, 21628360938574121ULL, 21817283854511261ULL, 22238558064758921ULL, 22318056296221571ULL, 22733842556089781ULL, 22849881428489231ULL, 23382987892499351ULL, 23417442472403711ULL, 25964083184094941ULL, 26515897161980111ULL, 29512383574028471ULL, 30074756036270831ULL, 30310618347929651ULL, 30402250951007051ULL, 30413977411117031ULL, 33502273017038711ULL, 33508988966488151ULL, 33976718302847051ULL, 34783522781262371ULL, 37564605737538611ULL, 37606024583356961ULL, 39138758504100371ULL, 40205947750578341ULL, 40257009922154141ULL, 40392614725338761ULL, 40504121267225501ULL, 41099072498143391ULL, 41289201480321911ULL, 41543933848913381ULL, 42218492028808211ULL, 43938526447515431ULL}}, // OEIS A213645 (12-tuples)
	{{0, 2, 6, 4, 2, 4, 2}, {1418575498589ULL, 27899359258019ULL, 34460918582339ULL, 76075560855389ULL, 186460616596349ULL, 218021188549259ULL, 234280497145559ULL, 282854319391739ULL, 345120905374109ULL, 346117552180649ULL, 604439135284079ULL, 727417501795079ULL, 1041814617748769ULL, 1090754719898939ULL, 1539765965257769ULL, 3152045700948239ULL, 3323127757029329ULL, 3449427485143889ULL, 4422879865247939ULL, 4525595253334019ULL, 4730773080017849ULL, 5462875671033029ULL, 6147764065076729ULL, 6205707895751459ULL, 6308411019731069ULL, 7582919852522879ULL, 7791180222409679ULL, 9162887985581579ULL, 9305359177794929ULL, 10096106139749879ULL, 10349085616714709ULL, 10744789916260649ULL, 10932016019429369ULL, 11140102475962709ULL, 12448240792011119ULL, 14727257011031429ULL, 16892267700442229ULL, 17963729763800069ULL, 18908121647739419ULL, 19028992697498879ULL, 19756696515786479ULL, 20252223877980959ULL, 20429666791949279ULL, 21680774776901489ULL, 21682173462980279ULL, 23076668788453529ULL, 24036602580170429ULL, 24101684579532809ULL, 25053289894907369ULL, 25309078073142959ULL, 25662701041982099ULL, 25777719656829389ULL, 26056424604564449ULL, 26315911419972269ULL, 26866456999592459ULL, 26887571851660769ULL, 27303559129791809ULL, 27839080743588209ULL, 28595465291933789ULL, 29137316070747749ULL, 30824439453812099ULL, 31395828815154899ULL, 31979851757518529ULL, 32897714831936819ULL}}, // OEIS A213601 (12-tuples) + 22
	// 8-tuples
	{{0, 2, 4, 2, 4, 6, 2, 6}, {380284918609481ULL, 437163765888581ULL, 701889794782061ULL, 980125031081081ULL, 1277156391416021ULL, 1487854607298791ULL, 1833994713165731ULL, 2115067287743141ULL, 2325810733931801ULL, 3056805353932061ULL, 3252606350489381ULL, 3360877662097841ULL, 3501482688249431ULL, 3595802556731501ULL, 3843547642594391ULL, 5000014653723821ULL, 5861268883004651ULL, 7486645325734691ULL, 7933248530182091ULL, 8760935349271991ULL, 8816939536219931ULL, 8871465225933041ULL, 9354490866900911ULL, 11096062107205961ULL, 11096434547374061ULL, 13764730155211151ULL, 13884748604026031ULL, 17438667992681051ULL, 20362378935668501ULL, 20471700514990841ULL, 20475715985020181ULL, 20614750499829371ULL, 21465425387541251ULL, 21628360938574121ULL, 21817283854511261ULL, 22238558064758921ULL, 22318056296221571ULL, 22733842556089781ULL, 22849881428489231ULL, 23382987892499351ULL, 23417442472403711ULL, 25964083184094941ULL, 26515897161980111ULL, 29512383574028471ULL, 30074756036270831ULL, 30310618347929651ULL, 30402250951007051ULL, 30413977411117031ULL, 33502273017038711ULL, 33508988966488151ULL, 33976718302847051ULL, 34783522781262371ULL, 37564605737538611ULL, 37606024583356961ULL, 39138758504100371ULL, 40205947750578341ULL, 40257009922154141ULL, 40392614725338761ULL, 40504121267225501ULL, 41099072498143391ULL, 41289201480321911ULL, 41543933848913381ULL, 42218492028808211ULL, 43938526447515431ULL}}, // OEIS A213645 (12-tuples)
	{{0, 2, 4, 6, 2, 6, 4, 2}, {1418575498577ULL, 27899359258007ULL, 34460918582327ULL, 76075560855377ULL, 186460616596337ULL, 218021188549247ULL, 234280497145547ULL, 282854319391727ULL, 345120905374097ULL, 346117552180637ULL, 604439135284067ULL, 727417501795067ULL, 1041814617748757ULL, 1090754719898927ULL, 1539765965257757ULL, 3152045700948227ULL, 3323127757029317ULL, 3449427485143877ULL, 4422879865247927ULL, 4525595253334007ULL, 4730773080017837ULL, 5462875671033017ULL, 6147764065076717ULL, 6205707895751447ULL, 6308411019731057ULL, 7582919852522867ULL, 7791180222409667ULL, 9162887985581567ULL, 9305359177794917ULL, 10096106139749867ULL, 10349085616714697ULL, 10744789916260637ULL, 10932016019429357ULL, 11140102475962697ULL, 12448240792011107ULL, 14727257011031417ULL, 16892267700442217ULL, 17963729763800057ULL, 18908121647739407ULL, 19028992697498867ULL, 19756696515786467ULL, 20252223877980947ULL, 20429666791949267ULL, 21680774776901477ULL, 21682173462980267ULL, 23076668788453517ULL, 24036602580170417ULL, 24101684579532797ULL, 25053289894907357ULL, 25309078073142947ULL, 25662701041982087ULL, 25777719656829377ULL, 26056424604564437ULL, 26315911419972257ULL, 26866456999592447ULL, 26887571851660757ULL, 27303559129791797ULL, 27839080743588197ULL, 28595465291933777ULL, 29137316070747737ULL, 30824439453812087ULL, 31395828815154887ULL, 31979851757518517ULL, 32897714831936807ULL}}, // OEIS A213601 (12-tuples) + 10
	{{0, 6, 2, 6, 4, 2, 4, 2}, {1418575498583ULL, 27899359258013ULL, 34460918582333ULL, 76075560855383ULL, 186460616596343ULL, 218021188549253ULL, 234280497145553ULL, 282854319391733ULL, 345120905374103ULL, 346117552180643ULL, 604439135284073ULL, 727417501795073ULL, 1041814617748763ULL, 1090754719898933ULL, 1539765965257763ULL, 3152045700948233ULL, 3323127757029323ULL, 3449427485143883ULL, 4422879865247933ULL, 4525595253334013ULL, 4730773080017843ULL, 5462875671033023ULL, 6147764065076723ULL, 6205707895751453ULL, 6308411019731063ULL, 7582919852522873ULL, 7791180222409673ULL, 9162887985581573ULL, 9305359177794923ULL, 10096106139749873ULL, 10349085616714703ULL, 10744789916260643ULL, 10932016019429363ULL, 11140102475962703ULL, 12448240792011113ULL, 14727257011031423ULL, 16892267700442223ULL, 17963729763800063ULL, 18908121647739413ULL, 19028992697498873ULL, 19756696515786473ULL, 20252223877980953ULL, 20429666791949273ULL, 21680774776901483ULL, 21682173462980273ULL, 23076668788453523ULL, 24036602580170423ULL, 24101684579532803ULL, 25053289894907363ULL, 25309078073142953ULL, 25662701041982093ULL, 25777719656829383ULL, 26056424604564443ULL, 26315911419972263ULL, 26866456999592453ULL, 26887571851660763ULL, 27303559129791803ULL, 27839080743588203ULL, 28595465291933783ULL, 29137316070747743ULL, 30824439453812093ULL, 31395828815154893ULL, 31979851757518523ULL, 32897714831936813ULL}}, // OEIS A213601 (12-tuples) + 16
	// 9-tuples
	{{0, 2, 4, 2, 4, 6, 2, 6, 4}, {380284918609481ULL, 437163765888581ULL, 701889794782061ULL, 980125031081081ULL, 1277156391416021ULL, 1487854607298791ULL, 1833994713165731ULL, 2115067287743141ULL, 2325810733931801ULL, 3056805353932061ULL, 3252606350489381ULL, 3360877662097841ULL, 3501482688249431ULL, 3595802556731501ULL, 3843547642594391ULL, 5000014653723821ULL, 5861268883004651ULL, 7486645325734691ULL, 7933248530182091ULL, 8760935349271991ULL, 8816939536219931ULL, 8871465225933041ULL, 9354490866900911ULL, 11096062107205961ULL, 11096434547374061ULL, 13764730155211151ULL, 13884748604026031ULL, 17438667992681051ULL, 20362378935668501ULL, 20471700514990841ULL, 20475715985020181ULL, 20614750499829371ULL, 21465425387541251ULL, 21628360938574121ULL, 21817283854511261ULL, 22238558064758921ULL, 22318056296221571ULL, 22733842556089781ULL, 22849881428489231ULL, 23382987892499351ULL, 23417442472403711ULL, 25964083184094941ULL, 26515897161980111ULL, 29512383574028471ULL, 30074756036270831ULL, 30310618347929651ULL, 30402250951007051ULL, 30413977411117031ULL, 33502273017038711ULL, 33508988966488151ULL, 33976718302847051ULL, 34783522781262371ULL, 37564605737538611ULL, 37606024583356961ULL, 39138758504100371ULL, 40205947750578341ULL, 40257009922154141ULL, 40392614725338761ULL, 40504121267225501ULL, 41099072498143391ULL, 41289201480321911ULL, 41543933848913381ULL, 42218492028808211ULL, 43938526447515431ULL}}, // OEIS A213645 (12-tuples)
	{{0, 2, 4, 6, 2, 6, 4, 2, 4}, {1418575498577ULL, 27899359258007ULL, 34460918582327ULL, 76075560855377ULL, 186460616596337ULL, 218021188549247ULL, 234280497145547ULL, 282854319391727ULL, 345120905374097ULL, 346117552180637ULL, 604439135284067ULL, 727417501795067ULL, 1041814617748757ULL, 1090754719898927ULL, 1539765965257757ULL, 3152045700948227ULL, 3323127757029317ULL, 3449427485143877ULL, 4422879865247927ULL, 4525595253334007ULL, 4730773080017837ULL, 5462875671033017ULL, 6147764065076717ULL, 6205707895751447ULL, 6308411019731057ULL, 7582919852522867ULL, 7791180222409667ULL, 9162887985581567ULL, 9305359177794917ULL, 10096106139749867ULL, 10349085616714697ULL, 10744789916260637ULL, 10932016019429357ULL, 11140102475962697ULL, 12448240792011107ULL, 14727257011031417ULL, 16892267700442217ULL, 17963729763800057ULL, 18908121647739407ULL, 19028992697498867ULL, 19756696515786467ULL, 20252223877980947ULL, 20429666791949267ULL, 21680774776901477ULL, 21682173462980267ULL, 23076668788453517ULL, 24036602580170417ULL, 24101684579532797ULL, 25053289894907357ULL, 25309078073142947ULL, 25662701041982087ULL, 25777719656829377ULL, 26056424604564437ULL, 26315911419972257ULL, 26866456999592447ULL, 26887571851660757ULL, 27303559129791797ULL, 27839080743588197ULL, 28595465291933777ULL, 29137316070747737ULL, 30824439453812087ULL, 31395828815154887ULL, 31979851757518517ULL, 32897714831936807ULL}}, // OEIS A213601 (12-tuples) + 10
	{{0, 4, 2, 4, 6, 2, 6, 4, 2}, {1418575498573ULL, 27899359258003ULL, 34460918582323ULL, 76075560855373ULL, 186460616596333ULL, 218021188549243ULL, 234280497145543ULL, 282854319391723ULL, 345120905374093ULL, 346117552180633ULL, 604439135284063ULL, 727417501795063ULL, 1041814617748753ULL, 1090754719898923ULL, 1539765965257753ULL, 3152045700948223ULL, 3323127757029313ULL, 3449427485143873ULL, 4422879865247923ULL, 4525595253334003ULL, 4730773080017833ULL, 5462875671033013ULL, 6147764065076713ULL, 6205707895751443ULL, 6308411019731053ULL, 7582919852522863ULL, 7791180222409663ULL, 9162887985581563ULL, 9305359177794913ULL, 10096106139749863ULL, 10349085616714693ULL, 10744789916260633ULL, 10932016019429353ULL, 11140102475962693ULL, 12448240792011103ULL, 14727257011031413ULL, 16892267700442213ULL, 17963729763800053ULL, 18908121647739403ULL, 19028992697498863ULL, 19756696515786463ULL, 20252223877980943ULL, 20429666791949263ULL, 21680774776901473ULL, 21682173462980263ULL, 23076668788453513ULL, 24036602580170413ULL, 24101684579532793ULL, 25053289894907353ULL, 25309078073142943ULL, 25662701041982083ULL, 25777719656829373ULL, 26056424604564433ULL, 26315911419972253ULL, 26866456999592443ULL, 26887571851660753ULL, 27303559129791793ULL, 27839080743588193ULL, 28595465291933773ULL, 29137316070747733ULL, 30824439453812083ULL, 31395828815154883ULL, 31979851757518513ULL, 32897714831936803ULL}}, // OEIS A213601 (12-tuples) + 6
	{{0, 4, 6, 2, 6, 4, 2, 4, 2}, {1418575498579ULL, 27899359258009ULL, 34460918582329ULL, 76075560855379ULL, 186460616596339ULL, 218021188549249ULL, 234280497145549ULL, 282854319391729ULL, 345120905374099ULL, 346117552180639ULL, 604439135284069ULL, 727417501795069ULL, 1041814617748759ULL, 1090754719898929ULL, 1539765965257759ULL, 3152045700948229ULL, 3323127757029319ULL, 3449427485143879ULL, 4422879865247929ULL, 4525595253334009ULL, 4730773080017839ULL, 5462875671033019ULL, 6147764065076719ULL, 6205707895751449ULL, 6308411019731059ULL, 7582919852522869ULL, 7791180222409669ULL, 9162887985581569ULL, 9305359177794919ULL, 10096106139749869ULL, 10349085616714699ULL, 10744789916260639ULL, 10932016019429359ULL, 11140102475962699ULL, 12448240792011109ULL, 14727257011031419ULL, 16892267700442219ULL, 17963729763800059ULL, 18908121647739409ULL, 19028992697498869ULL, 19756696515786469ULL, 20252223877980949ULL, 20429666791949269ULL, 21680774776901479ULL, 21682173462980269ULL, 23076668788453519ULL, 24036602580170419ULL, 24101684579532799ULL, 25053289894907359ULL, 25309078073142949ULL, 25662701041982089ULL, 25777719656829379ULL, 26056424604564439ULL, 26315911419972259ULL, 26866456999592449ULL, 26887571851660759ULL, 27303559129791799ULL, 27839080743588199ULL, 28595465291933779ULL, 29137316070747739ULL, 30824439453812089ULL, 31395828815154889ULL, 31979851757518519ULL, 32897714831936809ULL}}, // OEIS A213601 (12-tuples) + 12
	// 10-tuples
	{{0, 2, 4, 2, 4, 6, 2, 6, 4, 2}, {380284918609481ULL, 437163765888581ULL, 701889794782061ULL, 980125031081081ULL, 1277156391416021ULL, 1487854607298791ULL, 1833994713165731ULL, 2115067287743141ULL, 2325810733931801ULL, 3056805353932061ULL, 3252606350489381ULL, 3360877662097841ULL, 3501482688249431ULL, 3595802556731501ULL, 3843547642594391ULL, 5000014653723821ULL, 5861268883004651ULL, 7486645325734691ULL, 7933248530182091ULL, 8760935349271991ULL, 8816939536219931ULL, 8871465225933041ULL, 9354490866900911ULL, 11096062107205961ULL, 11096434547374061ULL, 13764730155211151ULL, 13884748604026031ULL, 17438667992681051ULL, 20362378935668501ULL, 20471700514990841ULL, 20475715985020181ULL, 20614750499829371ULL, 21465425387541251ULL, 21628360938574121ULL, 21817283854511261ULL, 22238558064758921ULL, 22318056296221571ULL, 22733842556089781ULL, 22849881428489231ULL, 23382987892499351ULL, 23417442472403711ULL, 25964083184094941ULL, 26515897161980111ULL, 29512383574028471ULL, 30074756036270831ULL, 30310618347929651ULL, 30402250951007051ULL, 30413977411117031ULL, 33502273017038711ULL, 33508988966488151ULL, 33976718302847051ULL, 34783522781262371ULL, 37564605737538611ULL, 37606024583356961ULL, 39138758504100371ULL, 40205947750578341ULL, 40257009922154141ULL, 40392614725338761ULL, 40504121267225501ULL, 41099072498143391ULL, 41289201480321911ULL, 41543933848913381ULL, 42218492028808211ULL, 43938526447515431ULL}}, // OEIS A213645 (12-tuples)
	{{0, 2, 4, 6, 2, 6, 4, 2, 4, 2}, {1418575498577ULL, 27899359258007ULL, 34460918582327ULL, 76075560855377ULL, 186460616596337ULL, 218021188549247ULL, 234280497145547ULL, 282854319391727ULL, 345120905374097ULL, 346117552180637ULL, 604439135284067ULL, 727417501795067ULL, 1041814617748757ULL, 1090754719898927ULL, 1539765965257757ULL, 3152045700948227ULL, 3323127757029317ULL, 3449427485143877ULL, 4422879865247927ULL, 4525595253334007ULL, 4730773080017837ULL, 5462875671033017ULL, 6147764065076717ULL, 6205707895751447ULL, 6308411019731057ULL, 7582919852522867ULL, 7791180222409667ULL, 9162887985581567ULL, 9305359177794917ULL, 10096106139749867ULL, 10349085616714697ULL, 10744789916260637ULL, 10932016019429357ULL, 11140102475962697ULL, 12448240792011107ULL, 14727257011031417ULL, 16892267700442217ULL, 17963729763800057ULL, 18908121647739407ULL, 19028992697498867ULL, 19756696515786467ULL, 20252223877980947ULL, 20429666791949267ULL, 21680774776901477ULL, 21682173462980267ULL, 23076668788453517ULL, 24036602580170417ULL, 24101684579532797ULL, 25053289894907357ULL, 25309078073142947ULL, 25662701041982087ULL, 25777719656829377ULL, 26056424604564437ULL, 26315911419972257ULL, 26866456999592447ULL, 26887571851660757ULL, 27303559129791797ULL, 27839080743588197ULL, 28595465291933777ULL, 29137316070747737ULL, 30824439453812087ULL, 31395828815154887ULL, 31979851757518517ULL, 32897714831936807ULL}}, // OEIS A213601 (12-tuples) + 10
	// 11-tuples
	{{0, 2, 4, 2, 4, 6, 2, 6, 4, 2, 4}, {380284918609481ULL, 437163765888581ULL, 701889794782061ULL, 980125031081081ULL, 1277156391416021ULL, 1487854607298791ULL, 1833994713165731ULL, 2115067287743141ULL, 2325810733931801ULL, 3056805353932061ULL, 3252606350489381ULL, 3360877662097841ULL, 3501482688249431ULL, 3595802556731501ULL, 3843547642594391ULL, 5000014653723821ULL, 5861268883004651ULL, 7486645325734691ULL, 7933248530182091ULL, 8760935349271991ULL, 8816939536219931ULL, 8871465225933041ULL, 9354490866900911ULL, 11096062107205961ULL, 11096434547374061ULL, 13764730155211151ULL, 13884748604026031ULL, 17438667992681051ULL, 20362378935668501ULL, 20471700514990841ULL, 20475715985020181ULL, 20614750499829371ULL, 21465425387541251ULL, 21628360938574121ULL, 21817283854511261ULL, 22238558064758921ULL, 22318056296221571ULL, 22733842556089781ULL, 22849881428489231ULL, 23382987892499351ULL, 23417442472403711ULL, 25964083184094941ULL, 26515897161980111ULL, 29512383574028471ULL, 30074756036270831ULL, 30310618347929651ULL, 30402250951007051ULL, 30413977411117031ULL, 33502273017038711ULL, 33508988966488151ULL, 33976718302847051ULL, 34783522781262371ULL, 37564605737538611ULL, 37606024583356961ULL, 39138758504100371ULL, 40205947750578341ULL, 40257009922154141ULL, 40392614725338761ULL, 40504121267225501ULL, 41099072498143391ULL, 41289201480321911ULL, 41543933848913381ULL, 42218492028808211ULL, 43938526447515431ULL}}, // OEIS A213645 (12-tuples)
	{{0, 4, 2, 4, 6, 2, 6, 4, 2, 4, 2}, {1418575498573ULL, 27899359258003ULL, 34460918582323ULL, 76075560855373ULL, 186460616596333ULL, 218021188549243ULL, 234280497145543ULL, 282854319391723ULL, 345120905374093ULL, 346117552180633ULL, 604439135284063ULL, 727417501795063ULL, 1041814617748753ULL, 1090754719898923ULL, 1539765965257753ULL, 3152045700948223ULL, 3323127757029313ULL, 3449427485143873ULL, 4422879865247923ULL, 4525595253334003ULL, 4730773080017833ULL, 5462875671033013ULL, 6147764065076713ULL, 6205707895751443ULL, 6308411019731053ULL, 7582919852522863ULL, 7791180222409663ULL, 9162887985581563ULL, 9305359177794913ULL, 10096106139749863ULL, 10349085616714693ULL, 10744789916260633ULL, 10932016019429353ULL, 11140102475962693ULL, 12448240792011103ULL, 14727257011031413ULL, 16892267700442213ULL, 17963729763800053ULL, 18908121647739403ULL, 19028992697498863ULL, 19756696515786463ULL, 20252223877980943ULL, 20429666791949263ULL, 21680774776901473ULL, 21682173462980263ULL, 23076668788453513ULL, 24036602580170413ULL, 24101684579532793ULL, 25053289894907353ULL, 25309078073142943ULL, 25662701041982083ULL, 25777719656829373ULL, 26056424604564433ULL, 26315911419972253ULL, 26866456999592443ULL, 26887571851660753ULL, 27303559129791793ULL, 27839080743588193ULL, 28595465291933773ULL, 29137316070747733ULL, 30824439453812083ULL, 31395828815154883ULL, 31979851757518513ULL, 32897714831936803ULL}}, // OEIS A213601 (12-tuples) + 6
	// 12-tuples
	{{0, 2, 4, 2, 4, 6, 2, 6, 4, 2, 4, 6}, {380284918609481ULL, 437163765888581ULL, 701889794782061ULL, 980125031081081ULL, 1277156391416021ULL, 1487854607298791ULL, 1833994713165731ULL, 2115067287743141ULL, 2325810733931801ULL, 3056805353932061ULL, 3252606350489381ULL, 3360877662097841ULL, 3501482688249431ULL, 3595802556731501ULL, 3843547642594391ULL, 5000014653723821ULL, 5861268883004651ULL, 7486645325734691ULL, 7933248530182091ULL, 8760935349271991ULL, 8816939536219931ULL, 8871465225933041ULL, 9354490866900911ULL, 11096062107205961ULL, 11096434547374061ULL, 13764730155211151ULL, 13884748604026031ULL, 17438667992681051ULL, 20362378935668501ULL, 20471700514990841ULL, 20475715985020181ULL, 20614750499829371ULL, 21465425387541251ULL, 21628360938574121ULL, 21817283854511261ULL, 22238558064758921ULL, 22318056296221571ULL, 22733842556089781ULL, 22849881428489231ULL, 23382987892499351ULL, 23417442472403711ULL, 25964083184094941ULL, 26515897161980111ULL, 29512383574028471ULL, 30074756036270831ULL, 30310618347929651ULL, 30402250951007051ULL, 30413977411117031ULL, 33502273017038711ULL, 33508988966488151ULL, 33976718302847051ULL, 34783522781262371ULL, 37564605737538611ULL, 37606024583356961ULL, 39138758504100371ULL, 40205947750578341ULL, 40257009922154141ULL, 40392614725338761ULL, 40504121267225501ULL, 41099072498143391ULL, 41289201480321911ULL, 41543933848913381ULL, 42218492028808211ULL, 43938526447515431ULL}}, // OEIS A213645
	{{0, 6, 4, 2, 4, 6, 2, 6, 4, 2, 4, 2}, {1418575498567ULL, 27899359257997ULL, 34460918582317ULL, 76075560855367ULL, 186460616596327ULL, 218021188549237ULL, 234280497145537ULL, 282854319391717ULL, 345120905374087ULL, 346117552180627ULL, 604439135284057ULL, 727417501795057ULL, 1041814617748747ULL, 1090754719898917ULL, 1539765965257747ULL, 3152045700948217ULL, 3323127757029307ULL, 3449427485143867ULL, 4422879865247917ULL, 4525595253333997ULL, 4730773080017827ULL, 5462875671033007ULL, 6147764065076707ULL, 6205707895751437ULL, 6308411019731047ULL, 7582919852522857ULL, 7791180222409657ULL, 9162887985581557ULL, 9305359177794907ULL, 10096106139749857ULL, 10349085616714687ULL, 10744789916260627ULL, 10932016019429347ULL, 11140102475962687ULL, 12448240792011097ULL, 14727257011031407ULL, 16892267700442207ULL, 17963729763800047ULL, 18908121647739397ULL, 19028992697498857ULL, 19756696515786457ULL, 20252223877980937ULL, 20429666791949257ULL, 21680774776901467ULL, 21682173462980257ULL, 23076668788453507ULL, 24036602580170407ULL, 24101684579532787ULL, 25053289894907347ULL, 25309078073142937ULL, 25662701041982077ULL, 25777719656829367ULL, 26056424604564427ULL, 26315911419972247ULL, 26866456999592437ULL, 26887571851660747ULL, 27303559129791787ULL, 27839080743588187ULL, 28595465291933767ULL, 29137316070747727ULL, 30824439453812077ULL, 31395828815154877ULL, 31979851757518507ULL, 32897714831936797ULL}} // OEIS A213601
};

struct MinerParameters {
	uint16_t threads, sieveWorkers, tupleLengthMin;
	uint64_t primorialNumber, primeTableLimit;
	bool useAvx2;
	uint64_t sieveBits, sieveSize, sieveWords, sieveIterations;
	std::vector<uint64_t> pattern, primorialOffsets;
	double restartDifficultyFactor;
	
	MinerParameters() :
		threads(0), sieveWorkers(0), tupleLengthMin(0),
		primorialNumber(0), primeTableLimit(0),
		useAvx2(false),
		sieveBits(0), sieveSize(0), sieveWords(0), sieveIterations(0),
		pattern{}, primorialOffsets{},
		restartDifficultyFactor(1.05) {}
};

class Options {
	MinerParameters _minerParameters;
	std::string _host, _username, _password, _mode, _payoutAddress, _secret, _tuplesFile;
	uint64_t _filePrimeTableLimit;
	uint16_t _debug, _port, _threads, _donate;
	double _refreshInterval, _difficulty, _benchmarkBlockInterval, _benchmarkTimeLimit;
	uint64_t _benchmarkPrimeCountLimit;
	std::vector<std::string> _rules;
	std::vector<std::string> _options;
	
	void _parseLine(std::string, std::string&, std::string&) const;
	void _stopConfig() const;
	
	public:
	Options() : // Default options: Standard Benchmark with 8 threads
		_host("127.0.0.1"),
		_username(""),
		_password(""),
		_mode("Benchmark"),
		_payoutAddress("ric1qr3yxckxtl7lacvtuzhrdrtrlzvlydane2h37ja"),
		_secret("/rM0.92/"),
		_tuplesFile("Tuples.txt"),
		_filePrimeTableLimit(0),
		_debug(0),
		_port(28332),
		_donate(2),
		_refreshInterval(30.),
		_difficulty(1024.),
		_benchmarkBlockInterval(150.),
		_benchmarkTimeLimit(86400.),
		_benchmarkPrimeCountLimit(1000000),
		_rules{"segwit"},
		_options{} {}
	
	void loadFileOptions(const std::string&, const bool);
	void loadCommandOptions(const int, char**);
	void parseOptions();
	
	MinerParameters minerParameters() const {return _minerParameters;}
	std::string mode() const {return _mode;}
	std::string host() const {return _host;}
	uint16_t port() const {return _port;}
	std::string username() const {return _username;}
	std::string password() const {return _password;}
	std::string payoutAddress() const {return _payoutAddress;}
	std::string secret() const {return _secret;}
	std::string tuplesFile() const {return _tuplesFile;}
	uint64_t filePrimeTableLimit() const {return _filePrimeTableLimit;}
	uint16_t donate() const {return _donate;}
	double refreshInterval() const {return _refreshInterval;}
	double difficulty() const {return _difficulty;}
	double benchmarkBlockInterval() const {return _benchmarkBlockInterval;}
	double benchmarkTimeLimit() const {return _benchmarkTimeLimit;}
	uint64_t benchmarkPrimeCountLimit() const {return _benchmarkPrimeCountLimit;}
	std::vector<std::string> rules() const {return _rules;}
};

#endif
