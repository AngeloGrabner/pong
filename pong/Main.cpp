#include "Scroll.h"

struct Rect
{
	olc::vf2d pos, size;
};

struct Ball
{
	Rect form;
	olc::vf2d velo;
	Ball()
	{
		form.size = { 8,8 };
	}
};

inline bool overlaps(Rect one, Rect other)
{
	return (one.pos.x < other.pos.x + other.size.x && one.pos.x + one.size.x >= other.pos.x &&
		one.pos.y < other.pos.y + other.size.y && one.pos.y + one.size.y >= other.pos.y);
}

class Pong : public olc::PixelGameEngine
{
	int mPHeight = 60, mScore1 = 0, mScore2 = 0;
	Rect mP1, mP2;
	std::vector<Ball> mBalls;
	float mPSpeed = 400;
	bool mRespawn = true;
	bool mP1v = true, mP2v = true;
	Scroll mLayers[3];
	bool OnUserCreate() override
	{
		mP1.pos.y = ScreenHeight()/ 2 - mPHeight/2;
		mP2.pos.y = ScreenHeight()/ 2 - mPHeight/2;
		mP1.pos.x = 4;
		mP2.pos.x = ScreenWidth() - 8;

		mP1.size.x = 4;
		mP2.size.x = 4;
		mP1.size.y = mPHeight;
		mP2.size.y = mPHeight;

		Ball ball;
		mBalls.push_back(ball);

		mLayers[0] = Scroll("l0.png", -0.3f, 2, ScreenWidth());
		mLayers[1] = Scroll("l1.png", -0.6f, 2, ScreenWidth());
		mLayers[2] = Scroll("l2.png", -1.5f, 2, ScreenWidth());
		return true;
	}
	bool OnUserUpdate(float ts) override
	{
		Input(ts);
		if (mRespawn)
		{
			for (auto& ball : mBalls)
			{
				GenBall(ball);
			}
			mRespawn = false;
		}

		for (auto& ball : mBalls)
		{
			ball.form.pos += ball.velo * ts;
			if (ball.form.pos.x <= 0)
			{
				mScore2++;
				mRespawn = true;
			}
			else if (ball.form.pos.x + ball.form.size.x >= ScreenWidth())
			{
				mScore1++;
				mRespawn = true;
			}

			if (overlaps(ball.form, mP1))
			{
				ball.velo.x = - ball.velo.x;
				ball.velo.x *= 1.04f;
				if (mP1v)
					ball.velo.y -= 12;
				else
					ball.velo.y += 12;
			}
			else if (overlaps(ball.form, mP2))
			{
				ball.velo.x = -ball.velo.x;
				ball.velo.x *= 1.04f;
				if (mP2v)
					ball.velo.y -= 12;
				else
					ball.velo.y += 12;
			}
			if (ball.form.pos.y <= 0 || ball.form.pos.y + ball.form.size.y >= ScreenHeight())
			{
				ball.velo.y = -ball.velo.y;
				ball.velo *= 1.05f;
			}
		}
		// draw
		for (int i = 0; i < 3; i++)
			mLayers[i].Draw(this, ts,{0,0}, {4.0f,4.0f});
		for (auto& ball : mBalls)
		{
			FillRectDecal(ball.form.pos + olc::vf2d{ 1.0f,1.0f }, ball.form.size, olc::BLACK);
			FillRectDecal(ball.form.pos, ball.form.size);
		}
		FillRectDecal(mP1.pos +olc::vf2d(1,1), mP1.size,olc::BLACK);
		FillRectDecal(mP1.pos, mP1.size);
		FillRectDecal(mP2.pos + olc::vf2d(1, 1), mP2.size, olc::BLACK);
		FillRectDecal(mP2.pos, mP2.size);

		olc::vf2d scale{ 2.5f, 2.5f };

		DrawStringDecal({ float(ScreenWidth() / 3 - 7),2.0f }, std::to_string(mScore1), olc::BLACK, scale);
		DrawStringDecal({ float(ScreenWidth() / 3 - 8 ),1.0f}, std::to_string(mScore1),olc::WHITE, scale);

		DrawStringDecal({ float(ScreenWidth() / 3 * 2 - 7), 2.0f }, std::to_string(mScore2), olc::BLACK, scale);
		DrawStringDecal({float( ScreenWidth() / 3 * 2 - 8), 1.0f }, std::to_string(mScore2), olc::WHITE, scale);

		for (int y = 0; y + 3 < ScreenHeight(); y+= 20)
		{
			FillRectDecal({ (ScreenWidth() / 2.0f) - 1,y + 1.0f }, { 2 ,10.0f },olc::BLACK);
			FillRectDecal({ (ScreenWidth() / 2.0f) - 2,(float)y }, { 2 ,10.0f });
		}
		return true;
	}
	void Input(float ts)
	{
		if (GetKey(olc::UP).bHeld && mP2.pos.y > 0)
		{
			mP2.pos.y -= mPSpeed * ts;
			mP2v = true;
		}
		if (GetKey(olc::DOWN).bHeld && mP2.pos.y + mP2.size.y < ScreenHeight())
		{
			mP2.pos.y += mPSpeed * ts;
			mP2v = false;
		}
		if ((GetKey(olc::W).bHeld || GetKey(olc::A).bHeld)&& mP1.pos.y > 0)
		{
			mP1.pos.y -= mPSpeed * ts;
			mP1v = true;
		}
		if ((GetKey(olc::S).bHeld || GetKey(olc::D).bHeld) && mP1.pos.y + mP1.size.y < ScreenHeight())
		{
			mP1.pos.y += mPSpeed * ts;
			mP1v = false;
		}
		if (GetKey(olc::NP1).bPressed)
		{
			Ball ball;
			GenBall(ball);
			mBalls.push_back(ball);
		}
		else if (GetKey(olc::NP0).bPressed)
		{
			if (mBalls.size() > 1)
				mBalls.pop_back();
		}
	}
	void GenBall(Ball& ball)
	{
		static bool flag = false;
		if (flag)
			ball.velo.x = rand() % 32 + 10;
		else
			ball.velo.x = rand() % 32 - 42;
		flag = !flag;
			ball.velo.y = rand() % 32 + 10;
		//ball.velo.clamp({-20,-20}, {20,20});

		ball.form.pos.x = ScreenWidth() / 2 - 1;
		ball.form.pos.y = rand() % ScreenHeight() - 2;
	}
};

int main()
{
	Pong pong;
	if (pong.Construct(480, 240, 2, 2, true, true))
		pong.Start();
	return 0;
}