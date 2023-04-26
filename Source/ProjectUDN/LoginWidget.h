// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableText.h"
#include "Components/Button.h"
#include "UObject/ConstructorHelpers.h"
#include "Json.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "LoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUDN_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void NativeConstruct();

    UPROPERTY(BlueprintReadWrite, Category = Test)
        UEditableText* Id;

    UPROPERTY(BlueprintReadWrite, Category = Test)
        UEditableText* Password;

    UPROPERTY(BlueprintReadWrite, Category = Test)
        FString FID;

    UPROPERTY(BlueprintReadWrite, Category = Test)
        FString FPassword;

    UFUNCTION(BlueprintCallable, meta = (keywords = "Login"))
        void OnLoginButtonClicked();

    UFUNCTION(BlueprintCallable, meta = (keywords = "SignUp"))
        void OnSignUpButtonClicked();

    UFUNCTION(BlueprintCallable, meta = (keywords = "SignUp"))
        void OnCreateSignUpButtonClicked();
    
    UFUNCTION(BlueprintCallable, meta = (keywords = "SignUp"))
        void OnBackButtonClicked();

    void HandleHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);
};