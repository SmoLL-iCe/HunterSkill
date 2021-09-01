#include <vector>
#include <string>

std::vector<std::string> monster_name =
{
"em001_00 Rathian",
"em001_01 Rathian Rosa",
"em001_02 Rathian Dourada",
"em002_00 Rathalos",
"em002_01 Rathalos Lazuli",
"em002_02 Rathalos Prateado",
"em007_00 Diablos",
"em007_01 Diablos Negra",
"em011_00 Kirin",
"em013_00 Fatalis",
"em018_00 Yian Garuga",
"em018_05 Yian Garuga Escoriado",
"em023_00 Rajang",
"em023_05 Rajang Furioso",
"em024_00 Kushala Daora",
"em026_00 Lunastra",
"em027_00 Teostra",
"em032_00 Tigrex",
"em032_01 Tigrex Bruto",
"em036_00 Lavasioth",
"em037_00 Nargacuga",
"em042_00 Barioth",
"em042_05 Barioth Presagelida",
"em043_00 Deviljho",
"em043_05 Deviljho Selvagem",
"em044_00 Barroth",
"em045_00 Uragaan",
"em050_00 Alatreon",
"em057_00 Zinogre",
"em057_01 Zinogre Tenebroso",
"em063_00 Brachydios",
"em063_05 Brachydios Colerico",
"em080_00 Glavenus",
"em080_01 Glavenus Acido",
"em100_00 Anjanath",
"em100_01 Anjanath Fulgurante",
"em101_00 Grande Jagras",
"em102_00 Pukei-Pukei",
"em102_01 Pukei-Pukei Coralino",
"em103_00 Nergigante",
"em103_05 Nergigante Ferrenho",
"em104_00 Safi'jiiva",
"em105_00 Xeno'jiiva",
"em106_00 Zorah Magdaros",
"em107_00 Kulu-Ya-Ku",
"em108_00 Jyuratodus",
"em109_00 Tobi-Kadachi",
"em109_01 Tobi-Kadachi Vipero",
"em110_00 Paolumu",
"em110_01 Paolumu Beladona",
"em111_00 Legiana",
"em111_05 Legiana Estridente",
"em112_00 Grande Girros",
"em113_00 Odogaron",
"em113_01 Odogaron Negro",
"em114_00 Radobaan",
"em115_00 Vaal Hazak",
"em115_05 Vaal Hazak Velado",
"em116_00 Dodogama",
"em117_00 Kulve Taroth",
"em118_00 Bazelgeuse",
"em118_05 Bazelgeuse Vulcanico",
"em120_00 Tzitzi-Ya-Ku",
"em121_00 Behemoth",
"em122_00 Beotodus",
"em123_00 Banbaro",
"em124_00 Velkhana",
"em125_00 Namielle",
"em126_00 Shara Ishvalda",
"em127_00 Liche",
"em127_01 Liche Anciao"
};

std::string getMonsterName( std::string file_monster )
{

    //std::string file_monster = entity.file;
    size_t erease = file_monster.find( "mod" );

    if ( erease != std::string::npos ) file_monster = file_monster.substr( erease + 4 );

    for ( const auto mh : monster_name )
    {
        size_t find = mh.find( file_monster );
        if ( find != std::string::npos )
        {
            file_monster = mh.substr( file_monster.length( ) + 1 );
            break;
        }
    }

    if ( !file_monster.compare( "em001_00 Rathian" ) )
        file_monster.clear( );

    return file_monster;
}