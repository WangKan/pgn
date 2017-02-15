#include "../Passes/Passes.h"
#include "PipeStates/LightIndexingPassesPipeStateDescs.h"
#include "PipeStates/LightingPassPipeStateDesc.h"
#include "PipeStates/LightVolumePassesPipeStateDescs.h"
#include "TechDesc.h"

static PipeStateDesc* pipeStateDescs[numPasses];

namespace {
	class Constructor
	{
	public:
		Constructor()
		{
			pipeStateDescs[LIGHT_VOLUME_BACK_FACE_PASS_1] = lightVolumeBackFacePassPipeStateDesc;
			pipeStateDescs[LIGHT_VOLUME_FRONT_FACE_PASS_1] = lightVolumeFrontFacePassPipeStateDesc;
			pipeStateDescs[LIGHT_INDEXING_PASS_1] = lightIndexingPass1PipeStateDesc;
			pipeStateDescs[LIGHT_VOLUME_BACK_FACE_PASS_2] = lightVolumeBackFacePassPipeStateDesc;
			pipeStateDescs[LIGHT_VOLUME_FRONT_FACE_PASS_2] = lightVolumeFrontFacePassPipeStateDesc;
			pipeStateDescs[LIGHT_INDEXING_PASS_2] = lightIndexingPass2PipeStateDesc;
			pipeStateDescs[LIGHT_VOLUME_BACK_FACE_PASS_3] = lightVolumeBackFacePassPipeStateDesc;
			pipeStateDescs[LIGHT_VOLUME_FRONT_FACE_PASS_3] = lightVolumeFrontFacePassPipeStateDesc;
			pipeStateDescs[LIGHT_INDEXING_PASS_3] = lightIndexingPass3PipeStateDesc;
			pipeStateDescs[LIGHT_VOLUME_BACK_FACE_PASS_4] = lightVolumeBackFacePassPipeStateDesc;
			pipeStateDescs[LIGHT_VOLUME_FRONT_FACE_PASS_4] = lightVolumeFrontFacePassPipeStateDesc;
			pipeStateDescs[LIGHT_INDEXING_PASS_4] = lightIndexingPass4PipeStateDesc;
		}
	};

	static Constructor constructor;
}

static TechDesc techDesc =
{
	pipeStateDescs
};

TechDesc* lightIndexingTechDesc = &techDesc;
