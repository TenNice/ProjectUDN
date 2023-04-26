// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"


void ULoginWidget::NativeConstruct()
{
    UButton* LoginButton = Cast<UButton>(GetWidgetFromName(TEXT("LoginButton")));

    UButton* SignUpButton = Cast<UButton>(GetWidgetFromName(TEXT("SignUpButton")));
    // 버튼 이벤트 핸들러 등록하기
    if (LoginButton)
    {
        LoginButton->OnClicked.AddDynamic(this, &ULoginWidget::OnLoginButtonClicked);

        UE_LOG(LogTemp, Warning, TEXT("로그인 버튼 이벤트야!!"));

    }
    if (SignUpButton)
    {
        SignUpButton->OnClicked.AddDynamic(this, &ULoginWidget::OnCreateSignUpButtonClicked);

        UE_LOG(LogTemp, Warning, TEXT("회원가입창으로 넘어가는 버튼 이벤트야!!"));

    }

    UButton* CreateSignUpButton = Cast<UButton>(GetWidgetFromName(TEXT("CreateSignUpButton")));

    UButton* BackButton = Cast<UButton>(GetWidgetFromName(TEXT("BackButton")));
    // 버튼 이벤트 핸들러 등록하기
    if (CreateSignUpButton)
    {
        CreateSignUpButton->OnClicked.AddDynamic(this, &ULoginWidget::OnSignUpButtonClicked);
    }
    if (BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &ULoginWidget::OnBackButtonClicked);
    }
}

void ULoginWidget::OnLoginButtonClicked()
{

    UE_LOG(LogTemp, Warning, TEXT("로그인 버튼 이벤트야"));
    // EditableText 값을 FString에 저장
    // Id & Password = FID & FPassword
    if (Id && Id->HasKeyboardFocus())
    {
        FID = Id->GetText().ToString();
    }
    else if (Password && Password->HasKeyboardFocus())
    {
        FPassword = Password->GetText().ToString();
    }

    // JSON 데이터 생성
    //TSharedPtr<FJsonObject>로 JSON 데이터 클래스 스마트포인터 생성
    //JsonObject는 스마트포인터를 가리키는 변수명
    //MakeShareable()는 인자로 받은 객체를 스마트 포인터가 관리하도록 만들어주는 함수
    //SetStringField(TEXT("email"), FID)는 email 문자열 필드에 FID값을 저장
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    JsonObject->SetStringField(TEXT("email"), FID);
    JsonObject->SetStringField(TEXT("password"), FPassword);
    
    // JSON 데이터를 문자열로 변환
    //JSON 데이터를 문자열로 저장하기 위해 생성
    //TJsonWriterFactory로 TJsonWriter<TCHAR> 클래스의 인스턴스 생성 &JsonString을 인자로 전달하여 JsonString에 JSON 데이터를 쓰는 데 사용
    //FJsonSerializer를 사용하여 JsonObject를 JsonWriter를 통해 문자열로 직렬화
    //Serialize() 함수는 JsonWriter를 통해 JSON 문자열을 생성하고, JsonString에 저장
    FString JsonString;
    TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

    // HTTP 요청 생성 및 설정
    //CreateRequest 함수를 호출하여 HTTP 요청 객체 생성, 해당 객체에 대한 TSharedRef 스마트 포인터 생성
    //HTTP 요청 객체의 메소드를 POST로 설정
    //HTTP 요청에 대한 헤더 중 Content-Type 헤더에 application/json 값 설정
    //HTTP 요청 URL을 해당 URL로 설정
    //HTTP 요청의 body에 JSON 문자열인 JsonString 설정
    TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();//요청 생성
    HttpRequest->SetVerb(TEXT("POST"));//요청의 구성요소
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));//요청의 구성요소
    HttpRequest->SetURL(TEXT("http://192.168.0.117:8080/api/login"));//요청의 구성요소
    HttpRequest->SetContentAsString(JsonString);//요청의 구성요소

    // 요청 완료 시 HandleHttpRequestComplete 함수 호출하여 결과 처리
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ULoginWidget::HandleHttpRequestComplete);

    // 생성된 HTTP 요청을 서버로 전송
    //이 메소드가 호출되지 않으면 서버로 전송 안됨
    HttpRequest->ProcessRequest();
}

//로그인 버튼과 동작방식 같음
//서버 URL에 따라 반환값(?)이 다를뿐
void ULoginWidget::OnSignUpButtonClicked()
{

    UE_LOG(LogTemp, Warning, TEXT("회원가입 하는 버튼 이벤트야"));
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

void ULoginWidget::OnCreateSignUpButtonClicked()
{
    // 현재 위젯 제거하기
    RemoveFromParent();

    UE_LOG(LogTemp, Warning, TEXT("회원가입창으로 넘어가는 버튼 이벤트야"));

    //    static ConstructorHelpers::FClassFinder<UCreateSignUpWidget> SignUpWidgetClass(TEXT("/Game/Widget/CreateSignUpWidget"));
    //    UCreateSignUpWidget* SignUpWidget = CreateWidget<UCreateSignUpWidget>(GetWorld(), SignUpWidgetClass.Class);
    //    if (SignUpWidget != nullptr)
    //    {
    //        // 생성된 위젯이 화면에 추가될 수 있도록 AddToViewport() 함수를 호출합니다.
    //        SignUpWidget->AddToViewport();
    //    }
    //    else
    //    {
    //        // 위젯 생성에 실패한 경우, 에러 메시지를 출력합니다.
    //        UE_LOG(LogTemp, Error, TEXT("Failed to create UMG_SignUp widget."));
    //    }


    /*CraeteSignUpViewPort();*/

    ////// UMG_SignUp 위젯 생성하기
    //static ConstructorHelpers::FClassFinder<UCreateSignUpWidget> SignUpWidgetClass(TEXT("/Game/Widget/CreateSignUpWidget"));
    //if (SignUpWidgetClass.Succeeded())
    //{
    //    UCreateSignUpWidget* SignUpWidget = CreateWidget<UCreateSignUpWidget>(GetWorld(), SignUpWidgetClass.Class);
    //    if (SignUpWidget != nullptr)
    //    {
    //        // 생성된 위젯이 화면에 추가될 수 있도록 AddToViewport() 함수를 호출합니다.
    //        SignUpWidget->AddToViewport();
    //    }
    //    else
    //    {
    //        // 위젯 생성에 실패한 경우, 에러 메시지를 출력합니다.
    //        UE_LOG(LogTemp, Error, TEXT("Failed to create UMG_SignUp widget."));
    //    }
    //}
    //else
    //{
    //    // UUMG_SignUp 클래스가 로드되지 않았을 경우, 에러 메시지를 출력합니다.
    //    UE_LOG(LogTemp, Error, TEXT("Failed to find UMG_SignUp class."));
    //}
}

void ULoginWidget::OnBackButtonClicked()
{


    // 현재 위젯 제거하기
    RemoveFromParent();

    UE_LOG(LogTemp, Warning, TEXT("뒤로가기 버튼이야"));


    //// UMG_Login 위젯 생성하기
    //static ConstructorHelpers::FClassFinder<UUMG_Login> LoginWidgetClass(TEXT("/Game/Widget/UMG_Login"));
    //if (LoginWidgetClass.Succeeded())
    //{
    //    UUMG_Login* LoginWidget = CreateWidget<UUMG_Login>(GetWorld(), LoginWidgetClass.Class);

    //    // UMG_Login 위젯 화면에 추가하기
    //    if (LoginWidget != nullptr)
    //    {
    //        LoginWidget->AddToViewport();

    //        UE_LOG(LogTemp, Warning, TEXT("로그인 위젯 뜸"));
    //    }
    //    else
    //    {
    //        UE_LOG(LogTemp, Warning, TEXT("로그인 위젯 안뜸"));
    //    }
    //}
    //else
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("그냥 다시 해"));
    //}
}