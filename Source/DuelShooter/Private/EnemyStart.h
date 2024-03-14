// All rights by IsGris

#pragma once

#include "CoreMinimal.h"
#include "Engine/NavigationObjectBase.h"
#include "EnemyStart.generated.h"

/**
 * Init position of the enemy in level
 */
UCLASS()
class DUELSHOOTER_API AEnemyStart : public ANavigationObjectBase
{
	GENERATED_BODY()
public:

	AEnemyStart();

#if WITH_EDITORONLY_DATA
private:
	UPROPERTY()
	TObjectPtr<class UArrowComponent> ArrowComponent;
public:
#endif

#if WITH_EDITORONLY_DATA
	/** Returns ArrowComponent subobject **/
	TObjectPtr<class UArrowComponent> GetArrowComponent() const;
#endif
};