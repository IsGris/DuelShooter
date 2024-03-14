// All rights by IsGris


#include "EnemyStart.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"

AEnemyStart::AEnemyStart() : Super()
{
	GetCapsuleComponent()->InitCapsuleSize( 40.0f, 92.0f );
	GetCapsuleComponent()->SetShouldUpdatePhysicsVolume( false );

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>( TEXT( "Arrow" ) );

	if ( !IsRunningCommandlet() && ArrowComponent )
	{
		ArrowComponent->ArrowColor = FColor( 150, 200, 255 );

		ArrowComponent->ArrowSize = 1.0f;
		ArrowComponent->bTreatAsASprite = true;
		ArrowComponent->SetupAttachment( GetCapsuleComponent() );
		ArrowComponent->bIsScreenSizeScaled = true;
	}

	bIsSpatiallyLoaded = false;
#endif // WITH_EDITORONLY_DATA
}

#if WITH_EDITORONLY_DATA
TObjectPtr<UArrowComponent> AEnemyStart::GetArrowComponent() const
{
	return ArrowComponent;
}
#endif // WITH_EDITORONLY_DATA