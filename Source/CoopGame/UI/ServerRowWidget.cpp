// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerRowWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MainMenuWidget.h"

void UServerRowWidget::OnClick_RowButton()
{
	Parent->SelectIndex(Index);
}

void UServerRowWidget::SetServerText(FText Text)
{
	ServerName->SetText(Text);
}

void UServerRowWidget::Setup(UMainMenuWidget* InParent, int32 InIndex)
{
	Parent = InParent;
	Index = InIndex;
	RowButton->OnClicked.AddDynamic(this, &UServerRowWidget::OnClick_RowButton);
}


