#include "InterpolationEx.h"



class FInterpolationExPlugin : public IInterpolationExPlugin
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE(FInterpolationExPlugin, InterpolationEx)



void FInterpolationExPlugin::StartupModule()
{
}

void FInterpolationExPlugin::ShutdownModule()
{
}
