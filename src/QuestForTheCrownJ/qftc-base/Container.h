#pragma once

template<typename T>
class Container
{
private:
	bool has_maximum;
	int quantity, maximum;

public:

	Container()
		: quantity(0), maximum(0), has_maximum(false)
	{
	}

	Container(T quantity)
		: quantity(quantity), maximum(0), has_maximum(false)
	{
	}

	Container(T quantity, T maximum)
		: quantity(quantity), maximum(maximum), has_maximum(true)
	{
	}

	bool IsFull() const
	{
		return has_maximum && maximum <= quantity;
	}

	bool IsEmpty() const
	{
		return quantity <= 0;
	}

	void Fill()
	{
		if (has_maximum)
			quantity = maximum;
	}

	T operator=(T value)
	{
		quantity = value;
		if (has_maximum && quantity >= maximum)
			quantity = maximum;
		return quantity;
	}
	bool operator>(T value) const { return quantity > value; }
	bool operator>=(T value) const { return quantity >= value; }
	bool operator<(T value) const { return quantity < value; }
	bool operator<=(T value) const { return quantity <= value; }
	bool operator==(T value) const { return quantity == value; }
	bool operator!=(T value) const { return quantity != value; }
	operator T() const { return quantity; }
};
