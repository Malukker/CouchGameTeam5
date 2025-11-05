// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuPauseRobotBattle.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API UMenuPauseRobotBattle : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* ResumeBtn;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* MainMenuBtn;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* OptionBtn;

	UFUNCTION()
	void ResumeGame();

	UFUNCTION()
	void MainMenuGame();

	UFUNCTION()
	void OptionGame();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> OptionMenu;

private:

	UPROPERTY()
	UUserWidget* SettingwidgetInstance;

	int32 CurrentIndex = 0;

	TArray<UButton*> ButtonList;

	void UpdateButtonFocus();
};
