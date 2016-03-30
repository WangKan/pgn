#pragma once
namespace pgn {

class AssetFactory;

class Asset
{
public:
	virtual AssetFactory* getFactory() = 0;
	virtual bool cook(void* rawData) = 0;
	virtual bool submit(void* rawData, void* customArg) = 0;
	virtual void unload(void* customArg) = 0; // ����ʧ���ٵ����Ҳû��
	virtual void* core() = 0; // δ���غû����ʧ�ܷ���0
};

}
