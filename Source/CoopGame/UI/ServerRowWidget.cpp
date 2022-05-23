// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerRowWidget.h"
#include "Components/TextBlock.h"

void UServerRowWidget::SetServerText(FText Text)
{
	ServerName->SetText(Text);
}