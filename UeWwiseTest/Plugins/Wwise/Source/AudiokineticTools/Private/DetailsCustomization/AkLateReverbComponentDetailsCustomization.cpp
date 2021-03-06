/********************************************************************************
The content of the files in this repository include portions of the AUDIOKINETIC
Wwise Technology released in source code form as part of the SDK package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use these files in accordance with the end user license agreement provided
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Copyright (c) 2021 Audiokinetic Inc.
********************************************************************************/

// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "AkLateReverbComponentDetailsCustomization.h"
#include "AkComponent.h"
#include "AkLateReverbComponent.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "ScopedTransaction.h"
#include "IPropertyUtilities.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "AudiokineticTools"


//////////////////////////////////////////////////////////////////////////
// FAkLateReverbComponentDetailsCustomization

FAkLateReverbComponentDetailsCustomization::FAkLateReverbComponentDetailsCustomization()
{
}

TSharedRef<IDetailCustomization> FAkLateReverbComponentDetailsCustomization::MakeInstance()
{
	return MakeShareable(new FAkLateReverbComponentDetailsCustomization());
}

void FAkLateReverbComponentDetailsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
	MyDetailLayout = &DetailLayout;
	TArray<TWeakObjectPtr<UObject>> ObjectsBeingCustomized;
	DetailLayout.GetObjectsBeingCustomized(ObjectsBeingCustomized);
	if (ObjectsBeingCustomized.Num() != 1)
	{
		return;
	}

	UAkLateReverbComponent* LateReverbBeingCustomized = Cast<UAkLateReverbComponent>(ObjectsBeingCustomized[0].Get());
	if (LateReverbBeingCustomized)
	{
		IDetailCategoryBuilder& ToggleDetailCategory = DetailLayout.EditCategory("Toggle");
		auto EnableHandle = DetailLayout.GetProperty("bEnable");
		EnableHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FAkLateReverbComponentDetailsCustomization::OnEnableValueChanged));

		if (!LateReverbBeingCustomized->bEnable)
		{
			DetailLayout.HideCategory("LateReverb");
		}
	}
}

void FAkLateReverbComponentDetailsCustomization::OnEnableValueChanged()
{
	MyDetailLayout->ForceRefreshDetails();
}

//////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE