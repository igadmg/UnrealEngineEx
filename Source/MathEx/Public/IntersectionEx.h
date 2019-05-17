#pragma once

#include "Math/Vector.h"



struct FIntersectionEx
{
	inline static bool LineCircleIntersection(const FVector2D& Start, const FVector2D& End, const FVector2D& Origin, float Radius, float& OutTime)
	{
		FVector2D A = (Start - Origin);
		FVector2D B = (End - Origin);

		return LineCircleIntersection(A, B, Radius, OutTime);
	}

	inline static bool LineCircleIntersection(const FVector2D& Start, const FVector2D& End, float Radius, float& OutTime)
	{
		float dX = End.X - Start.X;
		float dY = End.Y - Start.Y;
		float dR2 = dX * dX + dY * dY;
		float D = Start.X * End.Y - End.X * Start.Y;

		float d = Radius * Radius * dR2 - D * D;

		if (d < 0)
			return false;

		float sd = FMath::Sqrt(d);
		if (FMath::Abs(dX) > FMath::Abs(dY))
		{
			float x1 = (D * dY + dX * sd) / dR2;
			float x2 = (D * dY - dX * sd) / dR2;
			OutTime = FMath::Min((x1 - Start.X) / dX, (x2 - Start.X) / dX);
		}
		else
		{
			float y1 = (-D * dX + dY * sd) / dR2;
			float y2 = (-D * dX - dY * sd) / dR2;
			OutTime = FMath::Min((y1 - Start.Y) / dY, (y2 - Start.Y) / dY);
		}

		return true;
	}

	inline static bool LineSphereIntersection(const FVector& Start, const FVector& End, const FVector& Origin, float Radius, float& OutTime)
	{
		FVector Dir = (End - Start);
		float Length = Dir.Size(); Dir /= Length;
		return LineSphereIntersection(Start, Dir, Length, Origin, Radius, OutTime);
	}

	inline static bool LineSphereIntersection(const FVector& Start, const FVector& Dir, float Length, const FVector& Origin, float Radius, float& OutTime)
	{
		const FVector	EO = Start - Origin;
		const float		v = (Dir | (Origin - Start));
		const float		disc = Radius * Radius - ((EO | EO) - v * v);

		if (disc < 0.0f)
			return false;

		OutTime = (v - FMath::Sqrt(disc)) / Length;

		return OutTime >= 0.0f && OutTime <= 1.0f;
	}

	static inline bool LineBoxIntersection(const FBox& Box, const FVector& Start, const FVector& End, float& OutTime)
	{
		FVector StartToEnd = (End - Start).GetSafeNormal();
		return LineBoxIntersection(Box, Start, End, StartToEnd, StartToEnd.Reciprocal(), OutTime);
	}

	static inline bool LineBoxIntersection(const FBox& Box, const FVector& Start, const FVector& End, const FVector& StartToEnd, const FVector&	OneOverStartToEnd, float& OutTime)
	{
		FVector	Time;
		bool	bStartIsOutside = false;

		if (Start.X < Box.Min.X)
		{
			bStartIsOutside = true;
			if (End.X >= Box.Min.X)
			{
				Time.X = (Box.Min.X - Start.X) * OneOverStartToEnd.X;
			}
			else
			{
				return false;
			}
		}
		else if (Start.X > Box.Max.X)
		{
			bStartIsOutside = true;
			if (End.X <= Box.Max.X)
			{
				Time.X = (Box.Max.X - Start.X) * OneOverStartToEnd.X;
			}
			else
			{
				return false;
			}
		}
		else
		{
			Time.X = 0.0f;
		}

		if (Start.Y < Box.Min.Y)
		{
			bStartIsOutside = true;
			if (End.Y >= Box.Min.Y)
			{
				Time.Y = (Box.Min.Y - Start.Y) * OneOverStartToEnd.Y;
			}
			else
			{
				return false;
			}
		}
		else if (Start.Y > Box.Max.Y)
		{
			bStartIsOutside = true;
			if (End.Y <= Box.Max.Y)
			{
				Time.Y = (Box.Max.Y - Start.Y) * OneOverStartToEnd.Y;
			}
			else
			{
				return false;
			}
		}
		else
		{
			Time.Y = 0.0f;
		}

		if (Start.Z < Box.Min.Z)
		{
			bStartIsOutside = true;
			if (End.Z >= Box.Min.Z)
			{
				Time.Z = (Box.Min.Z - Start.Z) * OneOverStartToEnd.Z;
			}
			else
			{
				return false;
			}
		}
		else if (Start.Z > Box.Max.Z)
		{
			bStartIsOutside = true;
			if (End.Z <= Box.Max.Z)
			{
				Time.Z = (Box.Max.Z - Start.Z) * OneOverStartToEnd.Z;
			}
			else
			{
				return false;
			}
		}
		else
		{
			Time.Z = 0.0f;
		}

		if (bStartIsOutside)
		{
			OutTime = FMath::Max3(Time.X, Time.Y, Time.Z);

			if (OutTime >= 0.0f && OutTime <= 1.0f)
			{
				const FVector Hit = Start + StartToEnd * OutTime;
				const float BOX_SIDE_THRESHOLD = 0.1f;
				if (Hit.X > Box.Min.X - BOX_SIDE_THRESHOLD && Hit.X < Box.Max.X + BOX_SIDE_THRESHOLD &&
					Hit.Y > Box.Min.Y - BOX_SIDE_THRESHOLD && Hit.Y < Box.Max.Y + BOX_SIDE_THRESHOLD &&
					Hit.Z > Box.Min.Z - BOX_SIDE_THRESHOLD && Hit.Z < Box.Max.Z + BOX_SIDE_THRESHOLD)
				{
					return true;
				}
			}

			return false;
		}
		else
		{
			return true;
		}
	}
};
