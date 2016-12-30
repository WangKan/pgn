#include <string>
#include <vector>
#include <map>
#include <max.h>
#include <PGN/Assets/PHY.h>
#include "About.h"

class PhyExporter : public SceneExport, public ITreeEnumProc
{
public:
	static const int maxNumVerts = 30000;
	static const int maxNumIndices = 30000;

	Point3* pos;
	int numVertices;
	std::vector<unsigned short> indices;

	enum { exportPass } pass;
	PhyExporter();
	virtual ~PhyExporter();

	virtual int ExtCount() { return 1; }
	virtual const MCHAR* Ext(int n) { return L"phy"; }
	virtual const MCHAR* LongDesc() { return L"PGN PhysicsMesh File"; }
	virtual const MCHAR* ShortDesc() { return L"PGN PhysicsMesh File"; }
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

PhyExporter::PhyExporter()
{
	pos = new Point3[maxNumVerts];
}

PhyExporter::~PhyExporter()
{
	delete[] pos;
}

int PhyExporter::callback(INode *node)
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
				}
			}
			return TREE_CONTINUE;
		}
		return TREE_IGNORECHILDREN;
	};

	return TREE_IGNORECHILDREN;
}

int PhyExporter::DoExport(const MCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts, DWORD options)
{
	pass = exportPass;

	numVertices = 0;
	indices.clear();
	
	if (ei->theScene->EnumTree(this))
		return IMPEXP_FAIL;

	for (int i = 0; i < numVertices; i++)
	{
		pos[i] = Point3(pos[i].x, pos[i].z, pos[i].y);
	}

	pgn::PHYHeader header;

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

	fseek(f, 0, SEEK_SET);
	fwrite(&header, sizeof(header), 1, f);
	fclose(f);

	return IMPEXP_SUCCESS;
}

class PhyExporterDesc : public ClassDesc
{
	virtual int IsPublic() { return TRUE; }
	virtual void* Create(BOOL loading) { return new PhyExporter; }
	virtual const MCHAR* ClassName() { return L"Phy Exporter"; }
	virtual SClass_ID SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
	virtual Class_ID ClassID() { return Class_ID(0x599f61a1, 0x5a7c3836); }
	virtual const MCHAR* Category() { return L""; }
} _phyExporterDesc;

ClassDesc* phyExporterDesc = &_phyExporterDesc;
