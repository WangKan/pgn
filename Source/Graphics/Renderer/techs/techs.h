
struct TechDesc;

enum TechEnum
{
	LIGHT_INDEXING_TECH,
	STATIC_MESH_TECH,
	SKINNED_MESH_TECH,
	NAV_MESH_TECH,
	POST_PROCESSING_TECH,

	numTechs
};

extern TechDesc* techDescs[numTechs];
