// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/ToolBoxFunctionLibrary.h"

#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"

void UToolBoxFunctionLibrary::SlateNotification(const FText& NotificationText, bool IsSuccess,const FText& SubText)
{
	FNotificationInfo Info(NotificationText);
	Info.ExpireDuration = 1.f;
	Info.bUseSuccessFailIcons = true;
	Info.Image= FCoreStyle::Get().GetBrush("Icons.SuccessWithColor");
	Info.SubText= SubText;
	TSharedPtr<SNotificationItem> NotificationItem =FSlateNotificationManager::Get().AddNotification(Info);
	if (IsSuccess)
	{
		NotificationItem->SetCompletionState(SNotificationItem::CS_Success);
	}else
	{
		NotificationItem->SetCompletionState(SNotificationItem::CS_Fail);
	}
	
}
