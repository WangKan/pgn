namespace pgn {
	struct Float4x4;
}

void buildPerspectiveProjMatLH(pgn::Float4x4* mat, float w, float h, float n, float f);
void buildPerspectiveProjMatFovLH(pgn::Float4x4* mat, float fovY, float aspectRatio, float n, float f);
void buildOrthogonalProjMatLH(pgn::Float4x3* mat, float w, float h, float n, float f);

void buildMatRotationX(pgn::Float4x3* mat, float angle);
void buildMatRotationY(pgn::Float4x3* mat, float angle);
void buildMatRotationZ(pgn::Float4x3* mat, float angle);
