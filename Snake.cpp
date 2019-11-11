#include "Snake.h"
#include <cstdlib> 
#include <ctime>
#include <iostream>



Zone::Zone()
{
	width = 0;
	height = 0;
}


Zone::Zone(const Zone& other)
{
	height = other.height;
	width = other.width;
	Fruit = other.Fruit;
	
	zone = other.zone;

}

Zone& Zone::operator=(const Zone& other)
{

	height = other.height;
	width = other.width;
	zone = other.zone;
	return *this;
}


Zone::Zone(size_t _width, size_t _height)
{
	width = _width;
	height = _height;
	std::string _zone(height * width, ' ');
	zone = _zone;
	
	for (size_t i = 0; i < width * height; i++)
	{

		if (i >= 3 * width && i < width * (height - 1) && i % width == 0)
		{
			zone[i] = '|';
			zone[i - 1] = '|';
		}

		else if (i < width || (2 * width <= i && i < 3 * width) || (height-1)*width <= i && i <= height*width)
		{
			zone[i] = '=';
		}


		zone[3 * width - 1] = '='; //lazy solutions to small details
		zone[width * (height - 1) - 1] = '|';
	}
	
	size_t x = 2 + (size_t)((width - 2) * rand() / (RAND_MAX + 1.0));
	size_t y = 4 + (size_t)((height - 1) * rand() / (RAND_MAX + 1.0));
	Fruit.push_back(x);
	Fruit.push_back(y);
	zone[x + y * width] = '%';
}


void Zone::Display()
{
	std::cout << zone << std::endl;
}

void Zone::SpawnFruit()
{
	zone[Fruit[0] + Fruit[1] * width] = ' ';
	Fruit[0] = 2 + (size_t)((width - 3) * rand() / (RAND_MAX + 1.0));
	Fruit[1] = 4 + (size_t)((height - 6) * rand() / (RAND_MAX + 1.0));
	while (zone[Fruit[0] + Fruit[1] * width] != 'O' || Fruit[0] + Fruit[1] * width != '@')
	{
		Fruit[0] = 2 + (size_t)((width - 3) * rand() / (RAND_MAX + 1.0));
		Fruit[1] = 4 + (size_t)((height - 6) * rand() / (RAND_MAX + 1.0));
	}
	zone[Fruit[0] + Fruit[1] * width] = '%';
}

Snake::Snake()
{
	fitness = 0;
	size_t _sizes[] = { 21, 10, 3 };
	brain = Network(3, _sizes);
	std::vector<std::vector<size_t>> _body(0);
	body = _body;
	direction = 'S';
	Matrix _surroundings(24, 1);
	Surroundings = _surroundings;
	Surroundings = Matrix::uniform(Surroundings, 0);

	
}

Snake::Snake(Zone& zone)
{
	fitness = 0;
	size = 3;
	size_t dir = 1 + (size_t)(4 * rand() / (RAND_MAX + 1.0));
	size_t _sizes[] = {21, 10, 3};

	brain = Network(3, _sizes);
	Matrix _surroundings(24, 1);
	Surroundings = _surroundings;
	Surroundings = Matrix::uniform(Surroundings, 0);

	switch (dir)
	{
	case 1:
		direction = 'W'; break;
	case 2:
		direction = 'N'; break;
	case 3:
		direction = 'E'; break;
	case 4:
		direction = 'S'; break;
	};
	
	size_t x = 4 + (size_t)((zone.width - 7) * rand() / (RAND_MAX + 1.0));
	size_t y = 6 + (size_t)((zone.height - 9) * rand() / (RAND_MAX + 1.0));

	std::vector<size_t> head; 
	head.push_back(x);
	head.push_back(y);
	std::vector<std::vector<size_t>> _body(0);
	body = _body;
	body.push_back(head);
	zone.zone[head[0] + head[1] * zone.width] = '@';
	
	std::vector<size_t> part(2, 0);
	std::vector<size_t> tail(2, 0);
	
	switch (direction)
	{
	case 'W':
		part[0] = body[0][0] + 1;
		part[1] = body[0][1];
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O';
		tail[0] = body[0][0] + 2;
		tail[1] = body[0][1];
		body.push_back(tail);
		zone.zone[tail[0] + tail[1] * zone.width] = 'O'; break;


	case 'N':
		part[0] = body[0][0];
		part[1] = body[0][1] + 1;
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O';
		tail[0] = body[0][0];
		tail[1] = body[0][1] + 2;
		body.push_back(tail);
		zone.zone[tail[0] + tail[1] * zone.width] = 'O'; break;


	case 'E':
		part[0] = body[0][0] - 1;
		part[1] = body[0][1];
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O';
		tail[0] = body[0][0] - 2;
		tail[1] = body[0][1];
		body.push_back(tail);
		zone.zone[tail[0] + tail[1] * zone.width] = 'O'; break;

	
	case 'S':
		part[0] = body[0][0];
		part[1] = body[0][1] - 1;
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O';
		tail[0] = body[0][0];
		tail[1] = body[0][1] - 2;
		body.push_back(tail);
		zone.zone[tail[0] + tail[1] * zone.width] = 'O'; break;
	}

}

void Snake::Move(Zone& zone)
{
	movesleft--;
	std::vector<size_t> h(0);
	size_t x = 0;
	size_t y = 0;
	zone.zone[body[size - 1][0] + zone.width * body[size - 1][1]] = ' ';
	zone.zone[body[0][0] + zone.width * body[0][1]] = 'O';
	
	switch (direction)
	{
	case 'W':
		body[body.size()-1].pop_back();
		x = body[0][0] - 1;
		h.push_back(x);
		y = body[0][1];
		h.push_back(y);
		body.insert(body.begin(), h);
		zone.zone[body[0][0] + zone.width * body[0][1]] = '@'; break;

	case 'N':
		body[size - 1].pop_back();
		x = body[0][0];
		h.push_back(x);
		y = body[0][1] - 1;
		h.push_back(y);
		body.insert(body.begin(), h);
		zone.zone[body[0][0] + zone.width * body[0][1]] = '@'; break;

	case 'E':
		body[size - 1].pop_back();
		x = body[0][0] + 1;
		h.push_back(x);
		y = body[0][1];
		h.push_back(y);
		body.insert(body.begin(), h);
		zone.zone[body[0][0] + zone.width * body[0][1]] = '@'; break;

	case 'S':
		body[size - 1].pop_back();
		x = body[0][0];
		h.push_back(x);
		y = body[0][1] + 1;
		h.push_back(y);
		body.insert(body.begin(), h);
		zone.zone[body[0][0] + zone.width * body[0][1]] = '@'; break;
	}
}

void Snake::Decide()
{
	brain.FeedForward(Surroundings);
	Matrix output(3, 1);
	output = brain.GetOutput();
	double max = 0;
	size_t decision = 0;
	for (size_t i = 0; i < 3; i++)
	{
		if (max < output.at(i, 0))
		{
			max = output.at(i, 0);
			decision = i;
		}
	}
	if(decision == 0)
	{
		if (direction == 'W') { direction = 'S';}
		else if (direction == 'N') { direction = 'W';}
		else if (direction == 'E') { direction = 'N';}
		else { direction = 'E';}
	}
	else if (decision == 2)
	{
		if (direction == 'W') { direction = 'N'; }
		else if (direction == 'N') { direction = 'E'; }
		else if (direction == 'E') { direction = 'S'; }
		else { direction = 'W'; }
	}
	
}

void Snake::See(Zone& zone)
{
	double dist = zone.width; //returns 1 for no object in this direction, a normalized distance if there's anything (wall body fruit)

	for (size_t i = 0; i < body[0][0]; i++) //West 
	{
		if (zone.zone[i + zone.width * body[0][1]] == '%')
		{
			dist = abs((double)(i - body[0][0]));
			Surroundings.at(0, 0) = zone.width - dist / zone.width;
		}
		else if (zone.zone[i + zone.width * body[0][1]] == 'O')
		{
			dist = abs((double)(i - body[0][0]));
			Surroundings.at(1, 0) = zone.width - dist / zone.width;
		}
		else if (zone.zone[i + zone.width * body[0][1]] == '|')
		{
			dist = abs((double)(i - body[0][0]));
			Surroundings.at(2, 0) = zone.width - dist / zone.width;
		}
	}
	
	for (size_t i = zone.width - 1; i >= body[0][0]; i--) //East
	{
		if (zone.zone[i + zone.width * body[0][1]] == '%')
		{
			dist = abs((double)(i - body[0][0]));
			Surroundings.at(3, 0) = zone.width - dist / zone.width;
		}
		else if (zone.zone[i + zone.width * body[0][1]] == 'O')
		{
			dist = abs((double)(i - body[0][0]));
			Surroundings.at(4, 0) = zone.width - dist / zone.width;
		}
		else if (zone.zone[i + zone.width * body[0][1]] == '|')
		{
			dist = abs((double)(i - body[0][0]));
			Surroundings.at(5, 0) = zone.width - dist / zone.width;
		}
	}
	/*
	for (size_t i = 0; i < body[0][1]; i++) //North
	{
		if (zone.zone[body[0][0] + zone.width * i] == '%' && dist > abs((double)(i - body[0][0])))
		{
			dist = abs((double)(i - body[0][1]));
			Surroundings.at(6, 0) = zone.height - dist / zone.height;
		}
		else if (zone.zone[body[0][0] + zone.width * i] == 'O' && dist > abs((double)(i - body[0][0])))
		{
			dist = abs((double)(i - body[0][1]));
			Surroundings.at(7, 0) = zone.height - dist / zone.height;
		}
		else if (zone.zone[body[0][0] + zone.width * i] == '|' && dist > abs((double)(i - body[0][0])))
		{
			dist = abs((double)(i - body[0][1]));
			Surroundings.at(8, 0) = zone.height - dist / zone.height;
		}
	}

	for (size_t i = body[0][1]; i < zone.height; i++) //South
	{
		if (zone.zone[body[0][0] + zone.width * i] == '%' && dist > abs((double)(i - body[0][0])))
		{
			dist = abs((double)(i - body[0][1]));
			Surroundings.at(9, 0) = zone.height - dist / zone.height;
		}
		else if (zone.zone[body[0][0] + zone.width * i] == 'O' && dist > abs((double)(i - body[0][0])))
		{
			dist = abs((double)(i - body[0][1]));
			Surroundings.at(10, 0) = zone.height - dist / zone.height;
		}
		else if (zone.zone[body[0][0] + zone.width * i] == '|' && dist > abs((double)(i - body[0][0])))
		{
			dist = abs((double)(i - body[0][1]));
			Surroundings.at(11, 0) = zone.height - dist / zone.height;
		}
	}

	int b = body[0][1] - body[0][0];*/
	
}

void Snake::Check(Zone& zone)
{
	if (movesleft == 0) { dead = true; };
	switch (direction)
	{
	case 'W':
		if (zone.zone[body[0][0] - 1 + body[0][1] * zone.width] == '|' || zone.zone[body[0][0] - 1 + body[0][1] * zone.width] == '=' || zone.zone[body[0][0] - 1 + body[0][1] * zone.width] == 'O')
		{
			dead = true; 
		}break;
	case 'N':
		if (zone.zone[body[0][0] + (body[0][1] - 1) * zone.width] == '|' || zone.zone[body[0][0] - 1 + (body[0][1] - 1) * zone.width] == '=' || zone.zone[body[0][0] - 1 + (body[0][1] - 1) * zone.width] == 'O')
		{
			dead = true; 
		}break;
	case 'E':
		if (zone.zone[body[0][0] + 1 + body[0][1] * zone.width] == '|' || zone.zone[body[0][0] + 1 + body[0][1] * zone.width] == '=' || zone.zone[body[0][0] + 1 + body[0][1] * zone.width] == 'O')
		{
			dead = true; 
		}break;
	case 'S':
		if (zone.zone[body[0][0] + (body[0][1] + 1) * zone.width] == '|' || zone.zone[body[0][0] + (body[0][1] + 1) * zone.width] == '=' || zone.zone[body[0][0] + (body[0][1] + 1) * zone.width] == 'O')
		{
			dead = true; 
		}break;
	}
}

void Snake::Grow(Zone& zone)
{
	std::vector<size_t> h(0);
	switch (direction)
	{
	case 'W':
		if (zone.zone[body[0][0] - 1 + body[0][1] * zone.width] == '%')
		{
			size++;
			fitness++;
			movesleft = 200;
			zone.zone[body[0][0] + body[0][1] * zone.width] = 'O';
			h.push_back(body[0][0] - 1);
			h.push_back(body[0][1] * zone.width);
			body.insert(body.begin(), h);
			zone.zone[body[0][0] + body[0][1] * zone.width] = '@';
		}break;

	case 'N':
		if (zone.zone[body[0][0] + (body[0][1] - 1) * zone.width] == '%')
		{
			size++;
			fitness++;
			movesleft = 200;
			zone.zone[body[0][0] + body[0][1] * zone.width] = 'O';
			h.push_back(body[0][0]);
			h.push_back((body[0][1] - 1) * zone.width);
			body.insert(body.begin(), h);
			zone.zone[body[0][0] + body[0][1] * zone.width] = '@';
		}break;

	case 'E':
		if (zone.zone[body[0][0] + 1 + body[0][1] * zone.width] == '%')
		{
			size++;
			fitness++;
			movesleft = 200;
			zone.zone[body[0][0] + body[0][1] * zone.width] = 'O';
			h.push_back(body[0][0] + 1);
			h.push_back(body[0][1] * zone.width);
			body.insert(body.begin(), h);
			zone.zone[body[0][0] + body[0][1] * zone.width] = '@';
		}break;

	case 'S':
		if (zone.zone[body[0][0] + (body[0][1] + 1) * zone.width] == '%')
		{
			size++;
			fitness++;
			movesleft = 200;
			zone.zone[body[0][0] + body[0][1] * zone.width] = 'O';
			h.push_back(body[0][0]);
			h.push_back((body[0][1] + 1) * zone.width);
			body.insert(body.begin(), h);
			zone.zone[body[0][0] + body[0][1] * zone.width] = '@';
		}break;

	}
}

void Snake::DisplaySurroundings()
{
	Surroundings.display();
}