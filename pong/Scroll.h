#pragma once
#include "olcPixelGameEngine.h"
class Scroll
{
	float mSpeed = 0;
	olc::Sprite* mSprite = nullptr;
	olc::Decal* mDecal = nullptr;
	int mWidth = 0;
	int mPosSize = 0;
	olc::vf2d* mPos = nullptr;
public:
	Scroll() = default;
	Scroll(const std::string& path, float speed, const unsigned int bufferSize = 2, const unsigned int custumWidth = 0)
		: mSpeed(speed), mPosSize(bufferSize)
	{
		mSprite = new olc::Sprite(path);
		if (mSprite->width < 0 || mSprite->height < 0)
		{
			std::cout << "error: couldn't load " << path << std::endl;
			throw;
		}
		mDecal = new olc::Decal(mSprite);
		mWidth = mSprite->width;
		if (custumWidth > 0)
			mWidth = custumWidth;
		mPos = new olc::vf2d[mPosSize];
		for (int i = 0; i < mPosSize; i++)
		{
			if (mSpeed > 0)
				mPos[i] = olc::vf2d{ (float)(-mWidth * i),0 };
			else 
				mPos[i] = olc::vf2d{ (float)(mWidth * i),0 };
		}
	}
	Scroll(const Scroll&) = delete;
	Scroll(Scroll&&) = delete;
	~Scroll()
	{
		delete mSprite;
		delete mDecal;
		delete[] mPos;
	}
	Scroll& operator=(Scroll&& r) noexcept
	{
		if (this == &r)
			return *this;
		std::swap(this->mDecal, r.mDecal);
		std::swap(this->mPos, r.mPos);
		std::swap(this->mPosSize, r.mPosSize);
		std::swap(this->mSpeed, r.mSpeed);
		std::swap(this->mSprite, r.mSprite);
		std::swap(this->mWidth, r.mWidth);
		return *this;
	}
	void Draw(olc::PixelGameEngine* pge, float ts, const olc::vf2d& offset = {0,0}, const olc::vf2d& scale = { 1.01f, 1.0f})
	{
		pge->SetPixelMode(olc::Pixel::MASK);
		for (int i = 0; i < mPosSize; i++)
		{
			pge->DrawDecal(mPos[i]+offset, mDecal, scale);
			mPos[i].x += mSpeed * ts;
			if (mPos[i].x >= mWidth && mSpeed > 0)
			{
				mPos[i].x = - mWidth;
			}
			else if (mPos[i].x <= -mWidth && mSpeed < 0)
			{
				mPos[i].x = mWidth;
			}
		}
		pge->SetPixelMode(olc::Pixel::NORMAL);
	}

};