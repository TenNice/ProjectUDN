// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"

void ULoginWidget::OnLoginButtonClicked()
{
    // Id와 Password 입력값 저장
    if (Id && Id->HasKeyboardFocus())
    {
        FID = Id->GetText().ToString();
    }
    else if (Password && Password->HasKeyboardFocus())
    {
        FPassword = Password->GetText().ToString();
    }

    // JSON 데이터 생성
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    JsonObject->SetStringField(TEXT("email"), FID);
    JsonObject->SetStringField(TEXT("password"), FPassword);

    // JSON 데이터를 문자열로 변환
    FString JsonString;
    TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

    // HTTP 요청 생성 및 설정
    TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetVerb(TEXT("POST"));
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    HttpRequest->SetURL(TEXT("http://192.168.0.117:8080/api/login"));
    HttpRequest->SetContentAsString(JsonString);

    // 요청 완료 시 호출할 함수 설정
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ULoginWidget::HandleHttpRequestComplete);

    // 요청 전송
    HttpRequest->ProcessRequest();
}

void ULoginWidget::OnSignUpButtonClicked()
{
    // Id와 Password 입력값 저장
    if (Id && Id->HasKeyboardFocus())
    {
        FID = Id->GetText().ToString();
    }
    else if (Password && Password->HasKeyboardFocus())
    {
        FPassword = Password->GetText().ToString();
    }

    // JSON 데이터 생성
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    JsonObject->SetStringField(TEXT("email"), FID);
    JsonObject->SetStringField(TEXT("password"), FPassword);

    // JSON 데이터를 문자열로 변환
    FString JsonString;
    TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

    // HTTP 요청 생성 및 설정
    TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetVerb(TEXT("POST"));
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    HttpRequest->SetURL(TEXT("http://192.168.0.117:8080/api/signup"));
    HttpRequest->SetContentAsString(JsonString);

    // 요청 완료 시 호출할 함수 설정
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ULoginWidget::HandleHttpRequestComplete);

    // 요청 전송
    HttpRequest->ProcessRequest();
}

void ULoginWidget::HandleHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
    if (bSuccess && Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Response: %s"), *Response->GetContentAsString());

        // Convert response json to object
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());
        //if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
        //{
        //    // Get response fields
        //    bool bLoginSuccess = JsonObject->GetBoolField("success");
        //    FString message = JsonObject->GetStringField("message");

        //    if (bLoginSuccess)
        //    {
        //        UE_LOG(LogTemp, Warning, TEXT("Login success: %s"), *message);
        //    }
        //    else
        //    {
        //        UE_LOG(LogTemp, Warning, TEXT("Login failed: %s"), *message);
        //    }
        //}
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Http request failed"));
    }
}
