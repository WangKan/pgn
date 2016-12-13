namespace pgn {

struct Float4x3;
struct Float4x4;

}

void buildPerspectiveProjMatLH(pgn::Float4x4* mat, float w, float h, float n, float f);
void buildPerspectiveProjMatFovLH(pgn::Float4x4* mat, float verticalFov, float aspectRatio, float n, float f);
void buildOrthogonalProjMatLH(pgn::Float4x3* mat, float w, float h, float n, float f);
