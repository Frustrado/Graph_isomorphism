#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

using namespace std;

void set_all(int * target, size_t n, int value)
{
	for (size_t i = 0; i < n; ++i)
		target[i] = value;
}

void destroy(int ** target, size_t n)
{
	for (size_t i = 0; i < n; ++i)
		delete[] target[i];
	delete[] target;
}

int ** read_graph(string file, size_t &n)
{
    int ** graph;
    size_t node_c;
    int a, b;
	ifstream stream;
	stream.open(file, ios::in);
	if (!stream.good())
	{
		stream.close();
		throw logic_error("Nie moge otworzyc pliku");
	}
	if (!(stream >> node_c))
	{
        stream.close();
		throw logic_error("Nie moge przeczytac liczby wezlow");
	}
	n = node_c;
	graph = new int * [n];
	for (size_t i = 0; i < n; ++i)
	{
		graph[i] = new int[n];
		set_all(graph[i], n, 0);
	}
	while (stream >> a)
	{
		if (!(stream >> b))
		{
			stream.close();
			destroy(graph, n);
			throw logic_error("Nie moge przeczytac drugiego wezla w parze");
		}
		if (a < 0 || (size_t)a >= n)
		{
			stream.close();
			destroy(graph, n);
			throw logic_error("Niepoprawny indeks wezla");
		}
		if (b < 0 || (size_t)b >= n)
		{
			stream.close();
			destroy(graph, n);
			throw logic_error("Niepoprawny indeks wezla");
		}
		graph[a][b] = 1;
		graph[b][a] = 1;
	}

	stream.close();
	return graph;
}

void get_histo(int ** g, int * h, size_t n)
{
	for (size_t i = 0; i < n; ++i)
		for (size_t j = 0; j < n; ++j) h[i]+=g[i][j];
}

bool comp_histo(int * h1, int * h2, size_t n)
{
	int * _h1 = new int[n+1];
	set_all(_h1, n, 0);
	for (size_t i = 0; i < n; ++i) _h1[(size_t)h1[i]]++;

	int * _h2 = new int[n+1];
	set_all(_h2, n, 0);
	for (size_t i = 0; i < n; ++i) _h2[(size_t)h2[i]]++;

	for (size_t i = 0; i <= n; ++i)
		if (_h1[i] != _h2[i])
		{
			delete[] _h1;
			delete[] _h2;
			return false;
		}
	delete[] _h1;
	delete[] _h2;
	return true;
}

bool attempt(int ** g1, int ** g2, int * h1, int * h2, int * f, size_t n, int i, int c)
{
	if ((size_t)i == n) return true;
	for (size_t t = 0; t < n; ++t)
		if (f[t] == c)
			return false;
	f[(size_t)i] = c;
	if (h1[(size_t)i] != h2[(size_t)c])
	{
		f[(size_t)i] = -1;
		return false;
	}
	)
	for (size_t t = 0; t <= (size_t)i; ++t)
	{
		if (g1[(size_t)i][t] != g2[(size_t)c][(size_t)f[t]])
		{
			f[(size_t)i] = -1;
			return false;
		}
	}
	for (int t = 0; t < (int)n; ++t)
	{
		if (attempt(g1, g2, h1, h2, f, n, i + 1, t))
			return true;
	}
	f[(size_t)i] = -1;
	return false;
}

int main(int argc, char *argv[])
{
	string file_a;
	string file_b;

	if (argc != 3)
	{
		cout << "Niepoprawna liczba argumentow, uzywam standardowych" << endl;
		file_a = "g1.txt";
		file_b = "g2.txt";
	}
	else
	{
		file_a = argv[1];
		file_b = argv[2];
	}

	size_t n;
	size_t other_n;

	int ** g1 = 0;
	int ** g2 = 0;

	int * h1 = 0;
	int * h2 = 0;

	int * f = 0;

	try
	{
		g1 = read_graph(file_a, n);
		g2 = read_graph(file_b, other_n);

		if (n != other_n)
		{
			cout << "Nieizomorficzne" << endl;
		}
		else
		{
			h1 = new int[n];
			set_all(h1, n, 0);
			h2 = new int[n];
			set_all(h2, n, 0);
			get_histo(g1, h1, n);
			get_histo(g2, h2, n);
			if (!comp_histo(h1, h2, n))
			{
				cout << "Nieizomorficzne" << endl;
			}
			else
			{
				bool done = false;
				f = new int[n];
				set_all(f, n, -1);
				for (int c = 0; c < (int)n && !done; ++c)
					done = attempt(g1, g2, h1, h2, f, n, 0, c);
				if (done)
				{
					cout << "Izomorficzne" << endl;
					for (size_t i = 0; i < n; i++)
						cout << i << "-->" << f[i] << endl;
				}
				else
				{
					cout << "Nieizomorficzne" << endl;
				}
			}
		}
	}
	catch (logic_error ex)
	{
		cout << "Wyjatek:" << endl;
		cout << ex.what() << endl;
	}

	if (g1) destroy(g1, n);
	if (g2) destroy(g2, n);
	if (h1) delete[] h1;
	if (h2) delete[] h2;
	if (f)  delete[] f;

	return 0;
}
