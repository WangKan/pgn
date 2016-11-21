#include <PGN/Platform/DebugHeap.h>
#include <PGN/FileStream/StdFileStream.h>
#include <PGN/FileStream/ZipFileStream.h>
#include <PGN/Graphics/Camera.h>
#include <PGN/Graphics/Entity.h>
#include <PGN/Graphics/Graphics.h>
#include <PGN/Graphics/Model.h>
#include <PGN/Graphics/PointLight.h>
#include <PGN/Graphics/Scene.h>
#include <PGN/Graphics/SceneEntity.h>
#include <PGN/Graphics/ScenePointLight.h>
#include <PGN/Math/Math.h>
#include <PGN/Platform/UI/Window.h>
#include <PGN/Utilities/SkeletalAnimation/Animation.h>
#include <PGN/Utilities/SkeletalAnimation/AnimationFactory.h>
#include <PGN/Utilities/SkeletalAnimation/Skeleton.h>
#include <PGN/Utilities/SkeletalAnimation/SkeletonFactory.h>
#include <PGN/Utilities/SkeletalAnimation/SkeletonTemplate.h>
#include <PGN/Utilities/Clock.h>
#import <UIKit/UIKit.h>

pgn::Float4x3 viewMat =
{
    1,	0,	0,	0,
    0,	1,	0,	-1,
    0,	0,	1,	7,
};

pgn::Float4x3 worldMat =
{
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
};

pgn::Float4x3 worldMat2 =
{
    1, 0, 0, 2,
    0, 1, 0, 0,
    0, 0, 1, 0,
};

pgn::Float3 pos =
{
    -3, 2, -3
};

pgn::Float3 pos2 =
{
    3, 2, -3
};

pgn::Float3 pos3 =
{
    3, 2, 3
};

pgn::Float3 pos4 =
{
    -3, 2, 2
};

void logicThreadFunc(UIWindow* uiWnd)
{
	pgn::Clock* clock = pgn::Clock::create(1, 4800);
	int t = (int)clock->getTickCount();

	pgn::Window* wnd = pgn::Window::create(8, 8, 8, 8, 24, 8, 1, uiWnd);
    
    NSBundle* appBundle = [NSBundle mainBundle];
	pgn::FileStream* assetStream1 = pgn::createZipFileStream([[[appBundle resourcePath] stringByAppendingString:@"/"] UTF8String]);
    NSArray* paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString* cachesDir = paths[0];
	pgn::FileStream* cacheStream = pgn::createStdFileStream([[cachesDir stringByAppendingString:@"/"] UTF8String]);

	pgn::Graphics* graphics = pgn::Graphics::create(wnd->getDisplay(), assetStream1, /*cacheStream*/0);
	graphics->beginDraw(wnd);

	pgn::Scene* scene = graphics->createScene();

	pgn::PointLight* light = graphics->createPointLight();
    light->setIntensity(1.0f, 1.0f, 1.0f, 1.0f);
	light->setRadius(10);
	pgn::ScenePointLight* sceneLight = scene->add(light);
	sceneLight->setPosition(&pos);

    pgn::PointLight* light2 = graphics->createPointLight();
    light2->setIntensity(1.0f, 0.0f, 0.0f, 1.0f);
    light2->setRadius(10);
    pgn::ScenePointLight* sceneLight2 = scene->add(light2);
    sceneLight2->setPosition(&pos2);

    pgn::PointLight* light3 = graphics->createPointLight();
    light3->setIntensity(0.0f, 1.0f, 0.0f, 1.0f);
    light3->setRadius(10);
    pgn::ScenePointLight* sceneLight3 = scene->add(light3);
    sceneLight3->setPosition(&pos3);

    pgn::PointLight* light4 = graphics->createPointLight();
    light4->setIntensity(0.0f, 0.0f, 1.0f, 1.0f);
    light4->setRadius(10);
    pgn::ScenePointLight* sceneLight4 = scene->add(light4);
    sceneLight4->setPosition(&pos4);

	pgn::Camera* camera = graphics->createCamera();
	camera->setFrustumFovLH(60.0f/180.0f*3.14f, 4.0f/3.0f, 0.01f, 1024);
	camera->setViewMat(&viewMat);

	pgn::Entity* entity = graphics->createEntity();

    pgn::Model* model = graphics->createModel();
    model->setMesh("casual02_f_highpoly.PNM");
    model->setDiffuseMap(0, "casual02_f_25.PNT.zip>casual02_f_25.PNT");
    //model->setDiffuseMap(0, "gray");
    entity->setModel(model);

    pgn::Entity* entity2 = graphics->createEntity();

    pgn::Model* model2 = graphics->createModel();
    model2->setMesh("box_normal_flipped.PNM");
    model2->setDiffuseMap(0, "gray");
    entity2->setModel(model2);

	pgn::FileStream* assetStream2 = pgn::createStdFileStream([[[appBundle resourcePath] stringByAppendingString:@"/"] UTF8String]);

	pgn::SkeletonFactory* skelFactory = pgn::SkeletonFactory::create();
	pgn::Skeleton* skel = skelFactory->createSkeleton();
	pgn::AnimationFactory* animFactory = pgn::AnimationFactory::create(assetStream2);
	pgn::Animation* anim = animFactory->createAnimation();
	anim->set("idle1_f.PNA");
	skel->playAnimation(anim);
	entity->setSkeleton(skel);

	pgn::SceneEntity* sceneEntity = scene->add(entity, true);
	sceneEntity->setScale(0.01f, 0.01f);
	sceneEntity->setWorldMat(&worldMat);

	pgn::SceneEntity* sceneEntity2 = scene->add(entity, true);
	sceneEntity2->setScale(0.01f, 0.01f);
	sceneEntity2->setWorldMat(&worldMat2);

    pgn::SceneEntity* sceneEntity3 = scene->add(entity2, true);
    sceneEntity3->setScale(15.0f, 15.0f);
    sceneEntity3->setWorldMat(&worldMat);

    while (wnd->processMessages())
	{
		int _t = (int)clock->getTickCount();
		int dt = _t - t;
		t = _t;
		skel->updatePose(dt);
		camera->setViewport(0, 0, wnd->getClientWidth(), wnd->getClientHeight(), wnd->getClientHeight());
		scene->commit(camera);
	}

    scene->remove(sceneLight);
    scene->remove(sceneLight2);
    scene->remove(sceneLight3);
    scene->remove(sceneLight4);
    scene->remove(sceneEntity);
    scene->remove(sceneEntity2);
    scene->remove(sceneEntity3);
	scene->destroy();
    light->destroy();
    light2->destroy();
    light3->destroy();
    light4->destroy();
	camera->destroy();
	entity->destroy();
    model->destroy();
    entity2->destroy();
    model2->destroy();
	skel->destroy();
	skelFactory->destroy();
	anim->destroy();
	animFactory->destroy();
	pgn::destroyStdFileStream(assetStream2);

	graphics->endDraw();
	graphics->destroy();
	pgn::destroyStdFileStream(assetStream1);
	pgn::destroyStdFileStream(cacheStream);

	wnd->destroy();

	clock->destroy();
}
