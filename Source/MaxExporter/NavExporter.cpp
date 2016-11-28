#include <string>
#include <vector>
#include <map>
#include <max.h>
#include <PGN/Assets/NAV.h>
#include "About.h"

class NavExporter : public SceneExport, public ITreeEnumProc
{
public:
	static const int maxNumVerts = 30000;
	static const int maxNumIndices = 30000;

	Point3* pos;
	int numVertices;
	std::vector<unsigned short> indices;
	std::vector<unsigned short> adjacentNodes;

	enum { exportPass } pass;
	NavExporter();
	virtual ~NavExporter();

	virtual int ExtCount() { return 1; }
	virtual const MCHAR* Ext(int n) { return L"nav"; }
	virtual const MCHAR* LongDesc() { return L"PGN NavMesh File"; }
	virtual const MCHAR* ShortDesc() { return L"PGN NavMesh File"; }
	virtual const MCHAR* AuthorName() { return about.authorName.c_str(); }
	virtual const MCHAR* CopyrightMessage() { return about.copyright.c_str(); }
	virtual const MCHAR* OtherMessage1() { return L"OtherMessage1"; }
	virtual const MCHAR* OtherMessage2() { return L"OtherMessage2"; }
	virtual unsigned int Version() { return about.version; }
	virtual void ShowAbout(HWND hWnd) { /*MessageBox(hWnd, about.aboutText.c_str(), about.aboutCaption.c_str(), MB_OK);*/ }
	virtual int DoExport(const MCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts, DWORD options);

	// ITreeEnumProc
	virtual int callback(INode *node);
};

NavExporter::NavExporter()
{
	pos = new Point3[maxNumVerts];
}

NavExporter::~NavExporter()
{
	delete[] pos;
}

int getAdjacentSide(int a, int b, int p0, int p1, int p2)
{
	if ((a == p0 && b == p1) || (a == p1 && b == p0))
	{
		return 0;
	}

	if ((a == p1 && b == p2) || (a == p2 && b == p1))
	{
		return 1;
	}

	if ((a == p2 && b == p0) || (a == p0 && b == p2))
	{
		return 2;
	}
	return -1;
}

int NavExporter::callback(INode *node)
{
	switch (pass)
	{
	case exportPass:
		if (node->Selected())
		{
			Object* obj = node->EvalWorldState(0).obj;
			if (obj->SuperClassID() == GEOMOBJECT_CLASS_ID && obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
			{
				TriObject *triObj = (TriObject*)obj->ConvertToType(0, Class_ID(TRIOBJ_CLASS_ID, 0));
				Mesh* mesh = &triObj->mesh;
				int numFaces = mesh->numFaces;
				Face* faces = mesh->faces;
				Point3* verts = mesh->verts;
				Matrix3 worldMat = node->GetObjTMAfterWSM(0);

				indices.reserve(numFaces * 3);
				adjacentNodes.resize(numFaces * 3, -1);
				for (int faceIdx = 0; faceIdx < numFaces; faceIdx++)
				{
					Face* face = &faces[faceIdx];
					for (int corner = 0; corner < 3; corner++)
					{
						int vertIdx = face->v[corner];
						Point3 vert = verts[vertIdx] * worldMat;
						
						int i = 0;
						for (; i < numVertices; i++)
						{
							if (vert != pos[i]) continue;
							break;
						}

						if (i == numVertices)
						{
							pos[i] = vert;
							numVertices++;
						}

						indices.push_back(i);
					}

					for (int j = 0; j < indices.size(); j+=3)
					{
						if (j / 3 == faceIdx) continue;
						int side = getAdjacentSide(indices[faceIdx * 3], indices[faceIdx * 3 + 1], indices[j], indices[j + 1], indices[j + 2]);
						if (side >= 0)
						{
							adjacentNodes[faceIdx*3] = j / 3;
							adjacentNodes[j + side] = faceIdx;
							break;
						}
					}

					for (int j = 0; j < indices.size(); j += 3)
					{
						if (j / 3 == faceIdx) continue;
						int side = getAdjacentSide(indices[faceIdx * 3 + 1], indices[faceIdx * 3 + 2], indices[j], indices[j + 1], indices[j + 2]);
						if (side >= 0)
						{
							adjacentNodes[faceIdx*3 + 1] = j / 3;
							adjacentNodes[j + side] = faceIdx;
							break;
						}
					}

					for (int j = 0; j < indices.size(); j += 3)
					{
						if (j / 3 == faceIdx) continue;
						int side = getAdjacentSide(indices[faceIdx * 3 + 2], indices[faceIdx * 3], indices[j], indices[j + 1], indices[j + 2]);
						if (side >= 0)
						{
							adjacentNodes[faceIdx*3 + 2] = j / 3;
							adjacentNodes[j + side] = faceIdx;
							break;
						}
					}
				}
			}
			return TREE_CONTINUE;
		}
		return TREE_IGNORECHILDREN;
	};

	return TREE_IGNORECHILDREN;
}

int NavExporter::DoExport(const MCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts, DWORD options)
{
	pass = exportPass;

	numVertices = 0;
	indices.clear();
	adjacentNodes.clear();
	
	if (ei->theScene->EnumTree(this))
		return IMPEXP_FAIL;

	for (int i = 0; i < numVertices; i++)
	{
		pos[i] = Point3(pos[i].x, pos[i].z, pos[i].y);
	}

	pgn::NAVHeader header;

	FILE *f;
	_wfopen_s(&f, name, L"wb");
	fseek(f, sizeof(header), SEEK_SET);

	header.numVertices = numVertices;
	int offset = sizeof(header);
	header.verticesChunkOffset = offset;
	int size = sizeof(pos[0]) * numVertices;
	fwrite(pos, size, 1, f);
	offset += size;

	header.indicesChunkOffset = offset;
	header.numIndices = (int)indices.size();
	size = sizeof(indices[0]) * (int)indices.size();
	fwrite(&indices[0], size, 1, f);
	offset += size;

	header.adjacentNodesChunkOffset = offset;
	size = sizeof(adjacentNodes[0]) * (int)adjacentNodes.size();
	fwrite(&adjacentNodes[0], size, 1, f);

	fseek(f, 0, SEEK_SET);
	fwrite(&header, sizeof(header), 1, f);
	fclose(f);

	return IMPEXP_SUCCESS;
}

class NavExporterDesc : public ClassDesc
{
	virtual int IsPublic() { return TRUE; }
	virtual void* Create(BOOL loading) { return new NavExporter; }
	virtual const MCHAR* ClassName() { return L"NAV Exporter"; }
	virtual SClass_ID SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
	virtual Class_ID ClassID() { return Class_ID(0x27357d0c, 0x7a25482e); }
	virtual const MCHAR* Category() { return L""; }
} _navExporterDesc;

ClassDesc* navExporterDesc = &_navExporterDesc;
