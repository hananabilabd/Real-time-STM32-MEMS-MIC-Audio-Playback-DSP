

#include "arm_math.h"

float32_t testRefOutput_f32[320] = {

0.000000000000000000,	0.001898396760225296,	0.004215449094772339,	0.007432077080011368,	0.010948467999696732,	0.015026375651359558,	0.019191544502973557,	0.023574527353048325,	
0.027919445186853409,	0.032277785241603851,	0.036551639437675476,	0.040732793509960175,	0.044799156486988068,	0.048710610717535019,	0.052476800978183746,	0.056059073656797409,	
0.059482168406248093,	0.062726479023694992,	0.065821025520563126,	0.068763464689254761,	0.071577839553356171,	0.074270240962505341,	0.076856281608343124,	0.079344697296619415,	
0.081745062023401260,	0.084067162126302719,	0.086318407207727432,	0.088509257882833481,	0.090647127479314804,	0.092742368578910828,	0.094802625477313995,	0.096837285906076431,	
0.098853722214698792,	0.100859899073839190,	0.102862443774938580,	0.104867763817310330,	0.106881409883499150,	0.108908228576183320,	0.110952425748109820,	0.113017357885837550,	
0.115105822682380680,	0.117219865322113040,	0.119361080229282380,	0.121530555188655850,	0.123729091137647630,	0.125957202166318890,	0.128215309232473370,	0.130503740161657330,	
0.132822841405868530,	0.135173004120588300,	0.137554679065942760,	0.139968376606702800,	0.142414685338735580,	0.144894234836101530,	0.147407654672861100,	0.149955596774816510,	
0.152538605034351350,	0.155157200992107390,	0.157811731100082400,	0.160502441227436070,	0.163229387253522870,	0.165992442518472670,	0.168791320174932480,	0.171625509858131410,	
0.174494370818138120,	0.177397061139345170,	0.180332608520984650,	0.183299910277128220,	0.186297744512557980,	0.189324837177991870,	0.192379791289567950,	0.195461250841617580,	
0.198567759245634080,	0.201697919517755510,	0.204850304871797560,	0.208023533225059510,	0.211216274648904800,	0.214427210390567780,	0.217655111104249950,	0.220898788422346120,	
0.224157124757766720,	0.227429077029228210,	0.230713658034801480,	0.234009962528944020,	0.237317133694887160,	0.240634419023990630,	0.243961080908775330,	0.247296508401632310,	
0.250640105456113820,	0.253991369158029560,	0.257349837571382520,	0.260715119540691380,	0.264086868613958360,	0.267464816570281980,	0.270848698914051060,	0.274238351732492450,	
0.277633611112833020,	0.281034380197525020,	0.284440591931343080,	0.287852220237255100,	0.291269283741712570,	0.294691801071166990,	0.298119872808456420,	0.301553562283515930,	
0.304993014782667160,	0.308438356965780260,	0.311889752745628360,	0.315347377210855480,	0.318811416625976560,	0.322282072156667710,	0.325759567320346830,	0.329244095832109450,	
0.332735907286405560,	0.336235217750072480,	0.339742250740528110,	0.343257248401641850,	0.346780419349670410,	0.350311983376741410,	0.353852160274982450,	0.357401121407747270,	
0.360959105193614960,	0.364526227116584780,	0.368102725595235820,	0.371688675135374070,	0.375284302979707720,	0.378889638930559160,	0.382504884153604510,	0.386130042374134060,	
0.389765247702598570,	0.393410529941320420,	0.397065933793783190,	0.400731507688760760,	0.404407206922769550,	0.408093083649873730,	0.411789052188396450,	0.415495119988918300,	
0.419211201369762420,	0.422937240451574330,	0.426673140376806260,	0.430418811738491060,	0.434174135327339170,	0.437938995659351350,	0.441713258624076840,	0.445496778935194020,	
0.449289388954639430,	0.453090950846672060,	0.456901267170906070,	0.460720170289278030,	0.464547459036111830,	0.468382950872182850,	0.472226426005363460,	0.476077698171138760,	
0.479936532676219940,	0.483802750706672670,	0.487676106393337250,	0.491556398570537570,	0.495443399995565410,	0.499336875975131990,	0.503236617892980580,	0.507142387330532070,	
0.511053957045078280,	0.514971107244491580,	0.518893606960773470,	0.522821225225925450,	0.526753749698400500,	0.530690938234329220,	0.534632585942745210,	0.538578454405069350,	
0.542528338730335240,	0.546481993049383160,	0.550439231097698210,	0.554399792104959490,	0.558363504707813260,	0.562330115586519240,	0.566299438476562500,	0.570271246135234830,	
0.574245333671569820,	0.578221492469310760,	0.582199502736330030,	0.586179181933403020,	0.590160276740789410,	0.594142623245716090,	0.598125983029603960,	0.602110169827938080,	
0.606094967573881150,	0.610080175101757050,	0.614065583795309070,	0.618050977587699890,	0.622036151587963100,	0.626020893454551700,	0.630004994571208950,	0.633988231420516970,	
0.637970402836799620,	0.641951277852058410,	0.645930647850036620,	0.649908289313316350,	0.653883971273899080,	0.657857488840818410,	0.661828581243753430,	0.665797054767608640,	
0.669762641191482540,	0.673725124448537830,	0.677684243768453600,	0.681639779359102250,	0.685591462999582290,	0.689539063721895220,	0.693482317030429840,	0.697420965880155560,	
0.701354760676622390,	0.705283410847187040,	0.709206689149141310,	0.713124278932809830,	0.717035952955484390,	0.720941375941038130,	0.724840316921472550,	0.728732451796531680,	
0.732617516070604320,	0.736495196819305420,	0.740365199744701390,	0.744227230548858640,	0.748080968856811520,	0.751926124095916750,	0.755762357264757160,	0.759589381515979770,	
0.763406842947006230,	0.767214450985193250,	0.771011855453252790,	0.774798732250928880,	0.778574761003255840,	0.782339565455913540,	0.786092851310968400,	0.789834223687648770,	
0.793563373386859890,	0.797279909253120420,	0.800983514636754990,	0.804673787206411360,	0.808350402861833570,	0.812012966722249980,	0.815661124885082240,	0.819294504821300510,	
0.822912722826004030,	0.826515413820743560,	0.830102190375328060,	0.833672653883695600,	0.837226435542106630,	0.840763118118047710,	0.844282336533069610,	0.847783654928207400,	
0.851266715675592420,	0.854731071740388870,	0.858176350593566890,	0.861602116376161580,	0.865007970482110980,	0.868393491953611370,	0.871758259832859040,	0.875101849436759950,	
0.878423850983381270,	0.881723806262016300,	0.885001312941312790,	0.888255912810564040,	0.891487173736095430,	0.894694659858942030,	0.897877920418977740,	0.901036512106657030,	
0.904169965535402300,	0.907277844846248630,	0.910359673202037810,	0.913415014743804930,	0.916443370282649990,	0.919444311410188670,	0.922417331486940380,	0.925361987203359600,	
0.928277771919965740,	0.931164238601922990,	0.934020876884460450,	0.936847217381000520,	0.939642757177352910,	0.942407000809907910,	0.945139460265636440,	0.947839632630348210,	
0.950507018715143200,	0.953141096979379650,	0.955741371959447860,	0.958307322114706040,	0.960838429629802700,	0.963334184139966960,	0.965794049203395840,	0.968217510730028150,	
0.970604017376899720,	0.972953058779239650,	0.975264083594083790,	0.977536566555500030,	0.979769956320524220,	0.981963708996772770,	0.984117280691862110,	0.986230112612247470,	
0.988301653414964680,	0.990331344306468960,	0.992318630218505860,	0.994262944906950000,	0.996163722127676010,	0.998020399361848830,	0.999832402914762500,	1.001599155366420700,	
1.003320086747407900,	1.004994612187147100,	1.006622135639190700,	1.008202098309993700,	1.009733878076076500,	1.011216927319765100,	1.012650609016418500,	1.014034371823072400,	
1.015367589890956900,	1.016649682074785200,	1.017880033701658200,	1.019058048725128200,	1.020183108747005500,	1.021254621446132700,	1.022271949797868700,	1.023234523832798000,	

};
/* ----------------------------------------------------------------------
** Test input - logarithmic chirp signal
** ------------------------------------------------------------------- */

