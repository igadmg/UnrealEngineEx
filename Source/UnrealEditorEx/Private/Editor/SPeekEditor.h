#pragma once

#include "CoreMinimal.h"
//#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Toolkits/BaseToolkit.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Notifications/SErrorText.h"
#include "IDetailsView.h"
#include "Editor/PeekEdMode.h"
//#include "Toolkits/IToolkitHost.h"
//#include "AssetThumbnail.h"
//#include "Toolkits/BaseToolkit.h"


/*
class SLandscapeAssetThumbnail : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS( SLandscapeAssetThumbnail )
		: _ThumbnailSize( 64,64 ) {}
		SLATE_ARGUMENT( FIntPoint, ThumbnailSize )
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UObject* Asset, TSharedRef<FAssetThumbnailPool> ThumbnailPool);
	~SLandscapeAssetThumbnail();

	void SetAsset(UObject* Asset);

private:
	void OnMaterialCompilationFinished(UMaterialInterface* MaterialInterface);

	TSharedPtr<FAssetThumbnail> AssetThumbnail;
};
*/


class FPeekToolKit : public FModeToolkit
{
public:
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;

	/** Initializes the geometry mode toolkit */
	virtual void Init(const TSharedPtr< class IToolkitHost >& InitToolkitHost) override;

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FPeekEdMode* GetEditorMode() const override;
	virtual TSharedPtr<SWidget> GetInlineContent() const override;

	void NotifyToolChanged();
	


protected:
	void OnChangeMode(FName ModeName);
	bool IsModeEnabled(FName ModeName) const;
	bool IsModeActive(FName ModeName) const;

	void OnChangeTool(FName ToolName);
	bool IsToolEnabled(FName ToolName) const;
	bool IsToolActive(FName ToolName) const;

	

private:
	/** Geometry tools widget */
	TSharedPtr<class SPeekEditor> PeekEditorWidgets;
};



class SPeekEditor : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPeekEditor){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedRef<FPeekToolKit> InParentToolkit);

	void NotifyToolChanged();
	void NotifyBrushChanged();



protected:
	class FPeekEdMode* GetEditorMode() const;

	FText GetErrorText() const;

	bool GetPeekEditorIsEnabled() const;

	bool GetIsPropertyVisible(const FPropertyAndParent& PropertyAndParent) const;



protected:
	TSharedPtr<SErrorText> Error;

	TSharedPtr<IDetailsView> DetailsPanel;
};
