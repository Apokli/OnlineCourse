#pragma once
#include "DrawDebugHelpers.h"

#define DEBUG(content) if (GEngine) GEngine->AddOnScreenDebugMessage(1, 60., FColor::Red, content);
#define DEBUG_STRING(string) if (GEngine) GEngine->AddOnScreenDebugMessage(1, 60., FColor::Red, FString::Printf(TEXT(string)));
#define DEBUG_FLOAT(value) if (GEngine) GEngine->AddOnScreenDebugMessage(1, 60., FColor::Red, FString::Printf(TEXT("%f"), value));

#define DRAW_SPHERE_FIXED(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 24, FColor::Red, true);
#define DRAW_SPHERE(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 24, FColor::Red, false, -1.f);
#define DRAW_LINE_FIXED(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f);
#define DRAW_LINE(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f);
#define DRAW_POINT_FIXED(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, true);
#define DRAW_POINT(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, false, -1.f);
#define DRAW_VECTOR_FIXED(StartLocation, EndLocation) if (GetWorld()) \
{ \
DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f); \
DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Green, true); \
}
#define DRAW_VECTOR(StartLocation, EndLocation) if (GetWorld()) \
{ \
DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f); \
DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Green, false, -1.f); \
}
