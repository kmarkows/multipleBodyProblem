struct Velocity
{
	double x;
	double y;

	void operator=(Velocity &other)
	{
		x = other.x;
		y = other.y;
	}

	void setZeros()
	{
		x = 0;
		y = 0;
	}
};
