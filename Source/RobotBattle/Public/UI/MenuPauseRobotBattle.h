// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuPauseRobotBattle.generated.h"

class ARobotCharacter;
class APlayerMenuActor;
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

	UFUNCTION(BlueprintCallable, Category="Pause Menu")
	void BindToPlayerMenuActor(APlayerMenuActor* PlayerMenuActor);
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* ResumeBtn;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* MainMenuBtn;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* OptionBtn;

	UFUNCTION(BlueprintCallable, Category="Pause Menu")
	void ResumeGame();

	UFUNCTION(BlueprintCallable, Category="Pause Menu")
	void MainMenuGame();

	UFUNCTION(BlueprintCallable, Category="Pause Menu")
	void OptionGame();

	UFUNCTION(BlueprintCallable, Category="Pause Menu")
	void NavigationMenuUp();

	UFUNCTION(BlueprintCallable, Category="Pause Menu")
	void NavigationMenuDown();

	UFUNCTION(BlueprintCallable, Category="Pause Menu")
	void NavigationMenuValidate();

	UFUNCTION(BlueprintCallable, Category="Pause Menu")
	void NavigationMenuFocus();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> OptionMenu;

private:

	UFUNCTION()
	void OnMenuMoveInput(EPlayerMenuInputDirection Direction, APlayerController* Controller);

	UFUNCTION()
	void OnMenuValidateInput(APlayerController* Controller);

	
	UPROPERTY()
	UUserWidget* SettingwidgetInstance;

	int32 CurrentIndex = 0;

	TArray<UButton*> ButtonList;

	ARobotCharacter* Character;
};
