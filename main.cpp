#include <stdio.h>

#include <iostream>

#include <vector>

#include <set>

#include <map>

#include <math.h>

std::vector<std::string> split_ro(const std::string &str,
								  const char delimiter)
{
	std::vector<std::string> result;
	std::string current = "";

	for (size_t i = 0; i < str.size(); ++i)
	{
		if (str[i] == delimiter)
		{
			if (!current.empty())
			{
				result.push_back(current);
				current.clear();
			}
		}
		else
		{
			current += str[i];
		}
	}

	if (!current.empty())
	{
		result.push_back(current);
	}

	return result;
}

using namespace std;

// POWERED BY HUSKYDG @ THE VI

#define DEBUG 1

#if DEBUG == 2
#define dprintf(...) printf(__VA_ARGS__); getchar();
#elif DEBUG == 1
#define dprintf(...) printf(__VA_ARGS__)
#else
#define dprintf(...)
#endif

std::vector< char > _readline(FILE *_file) {
	std::vector<char> char_buffer;
	char c = 0;
	for (int i = 0; fread(&c, 1, 1, _file) > 0 && c != '\0' && c != '\n'; i++)
	{
		char_buffer.push_back(c);
	}
	char_buffer.push_back(0);
	return char_buffer;
}

struct dataset_list
{
	std::vector<std::string> names;
	std::vector< std::vector<std::string> > values;

	int pos(std::string name)
	{
		int i = -1;
		for (i = 0; i < names.size(); i++)
		{
			if (std::string(names[i]) == name)
				break;
		}
		return i;
	}

	std::string get(std::string name, int index)
	{
		int i = pos(name);
		if (i != -1)
		{
			return values[index][i];
		}
		return "";
	}

	void destroy_field(std::string name)
	{
		int i = -1;
		for (i = 0; i < names.size(); i++)
		{
			if (std::string(names[i]) == name)
				break;
		}
		if (i >= 0)
		{
			for (std::vector< std::vector<std::string> >::iterator it = values.begin(); it != values.end(); it++)
			{
				(*it).erase((*it).begin() + i);
			}
			names.erase(names.begin() + i);
		}
	}

	std::set<std::string> diff_data(std::string name)
	{
		return diff_data(pos(name));
	}

	std::set<std::string> diff_data(int i)
	{
		std::set<std::string> result;
		if (i != -1)
		{
			for (std::vector< std::vector<std::string> >::iterator it = values.begin(); it != values.end(); it++)
			{
				result.insert((*it)[i]);
			}
		}
		return result;
	}

	struct dataset_list loc_list(std::string name, std::string value)
	{
		struct dataset_list result
		{
			.names = names
		};
		for (std::vector< std::vector<std::string> >::iterator it = values.begin(); it != values.end(); it++)
		{
			if ((*it)[pos(name)] == value)
				result.values.push_back(*it);
		}
		result.destroy_field(name);
		return result;
	}

	int read_file(const char *filename, bool read_first_as_names = false)
	{
		
		FILE *_file = fopen(filename, "r");
		if (_file != NULL)
		{
			if (read_first_as_names) {
				names = split_ro(_readline(_file).data(), ',');
			}
			while (!feof(_file))
			{
				{
					std::vector<std::string> data = split_ro(_readline(_file).data(), ',');
					if (names.size() <= 0) {
						for (int e = 0; e < data.size(); e++) {
							char buf[20];
							sprintf(buf, "%d", e);
							names.push_back(buf);
						
						}
					}
					if (data.size() != names.size())
						continue;
					values.push_back(data);
				}
			};
			fclose(_file);
			return 0;
		}
		return 1;
	}

	void show()
	{
		int width[names.size()];
		for (int i = 0; i < names.size(); i++)
			width[i] = names[i].size();
		for (std::vector< std::vector<std::string> >::iterator it = values.begin(); it != values.end(); it++)
		{
			for (int i = 0; i < names.size(); i++)
				if (width[i] < ((*it)[i]).size())
				{
					width[i] = (*it)[i].size();
				}
		}
		printf(" %*s ", 10, "index");
		for (int i = 0; i < names.size(); i++)
			printf(" %*s ", -(width[i]), names[i].data());
		printf("\n");

		for (std::vector< std::vector<std::string> >::iterator it = values.begin(); it != values.end(); it++)
		{
			printf(" %*d ", 10, it - values.begin());
			for (int i = 0; i < names.size(); i++)
				printf(" %*s ", -(width[i]), (*it)[i].data());
			printf("\n");
		}
	}
	
	void clear() {
		names.clear();
		for (std::vector< std::vector<std::string> >::iterator it = values.begin(); it != values.end(); it++)
			(*it).clear();
		values.clear();
	}
};

struct linked_tree
{
	std::string value;
	struct tree *linked = NULL;
};

struct tree
{
	std::string node_name;
	std::vector<struct linked_tree> node_value;

	struct tree *traversal_tree(std::string value)
	{
		for (std::vector<struct linked_tree>::iterator it = node_value.begin(); it != node_value.end(); it++)
		{
			if ((*it).value == value)
				return (*it).linked;
		}
		return NULL;
	}

	void print_tree(int lv = 0, bool show_addr = false)
	{
		std::vector<char> fc;
		for (int i = 0; i < lv; i++)
			fc.push_back('\t');
		fc.push_back('\0');
		char back = '[', front = ']';
		if (node_value.size() <= 0)
		{
			back = '(';
			front = ')';
		}
		if (show_addr)
			printf("%s%X:%c%s%c\n", fc.data(), this, back, node_name.data(), front);
		else
			printf("%s%c%s%c\n", fc.data(), back, node_name.data(), front);
		// traversal
		for (std::vector<struct linked_tree>::iterator it = node_value.begin(); it != node_value.end(); it++)
		{
			printf("%s<%s>\n", fc.data(), (*it).value.data());
			if ((*it).linked == NULL)
				continue;
			(*it).linked->print_tree(lv + 1, show_addr);
		}
	}
};

void build_decision_tree(tree &_tree, dataset_list _dtlist)
{
	if (DEBUG)
		_dtlist.show();
	int pos_min = 0;
	{
		const int ROWS = _dtlist.names.size();
		std::set<std::string> outputs = _dtlist.diff_data(_dtlist.names.size() - 1);

		set<string> VALUES[ROWS - 1];
		int VALUES_COUNT = 0;

		for (int i = 0; i < ROWS - 1; i++)
		{
			VALUES[i] = _dtlist.diff_data(i);
			VALUES_COUNT += VALUES[i].size();
		}

		std::vector<float> H_purity[ROWS];

		// Tao bien de luu tru so lan xuat hien cua gia tri input output
		int occurences_count[VALUES_COUNT + outputs.size()];

		// Tao bien de luu tru so lan xuat hien cua output doi voi gia tri input
		int decision_count[VALUES_COUNT][outputs.size()];

		//init to 0
		for (int i = 0; i < VALUES_COUNT; i++)
		{
			occurences_count[i] = 0;
			for (int j = 0; j < outputs.size(); j++)
				decision_count[i][j] = 0;
		}
		for (int i = 0; i < outputs.size(); i++)
			occurences_count[VALUES_COUNT + i] = 0;

		///dprintf("xu ly so lieu...\n");
		// dong for nay de tao bang do cac gia tri can thiat de lat nua ta tinh gia tri H
		for (vector< vector<string> >::iterator it = _dtlist.values.begin(); it != _dtlist.values.end(); it++)
		{
			int s = 0, in_pos, out_pos;

			///dprintf("process output...\n");
			// tim vi tri cua output trong std::set<string> outputs
			std::set<std::string>::iterator nit = outputs.begin();
			for (out_pos = 0; out_pos < outputs.size(); out_pos++)
			{
				///dprintf("[%s] = [%s] ? \n", ( * nit).data(), ( * it)[( * it).size() - 1].data());
				if (*nit == (*it)[(*it).size() - 1])
				{
					///dprintf("break at pos %d\n", out_pos);
					break;
				}
				nit++;
			}
			///dprintf("process input...\n");
			for (int i = 0, max_size = VALUES[i].size(), pos; i < ROWS - 1; ++i) // tim vi tri cua decision_count de luu so lan xuat hien cua output doi voi input nay
			{
				std::set<std::string>::iterator vit = VALUES[i].begin();
				for (in_pos = 0; in_pos < max_size; in_pos++) // truoc tien can tim vi tri cua du lieu
				{
					///dprintf("[%s] = [%s] ? \n", ( * vit).data(), ( * it)[i].data());
					if (*(vit) == (*it)[i])
					{
						///dprintf("break at pos %d\n", in_pos);
						break;
					}
					vit++;
				}

				// tang bien dem len +1
				decision_count[s + in_pos][out_pos]++;
				occurences_count[s + in_pos]++;
				///dprintf("input %d == %d\n", s + in_pos, occurences_count[s + in_pos]);
				///dprintf("input:output %d:%d == %d\n", s + in_pos, out_pos, decision_count[s + in_pos][out_pos]);

				// dich vi tri them so gia tri khac nhau cua cot nay
				s += max_size;
			}

			// tang so lan xuat hien cua output
			occurences_count[VALUES_COUNT + out_pos]++;
		}

		// da chay den node la
		if (ROWS <= 1 || (outputs.size() <= 1 /* && VALUES_COUNT <= 1 /**/))
		{
			int max = occurences_count[VALUES_COUNT];
			int pos = 0;
			int update_time = 0;
			for (int i = 1; i < outputs.size(); i++)
			{
				if (occurences_count[VALUES_COUNT + i] > max)
				{
					max = occurences_count[VALUES_COUNT + i];
					pos = i;
					update_time++;
				}
			}
			if (DEBUG)
			{
				int i = 0;
				for (std::set<string>::iterator it = outputs.begin(); it != outputs.end(); it++)
				{
					printf("-> %s: %d\n", (*it).data(), occurences_count[VALUES_COUNT + i]);
					i++;
				}
			}

			std::set<std::string>::iterator it = outputs.begin();
			if (outputs.size() > 1 && update_time == 0)
			{
				// khi so lan update max = 0 ma van co nhieu hon 1 output, nghia la so lan xuat hien cac output la giong nhau
				// Vi du nhu:
				//  50% Yes : 50% No
				//  1/3 A : 1/3 B : 1/3 C
				//  25% low : 25% balance : 25% high : 25 extreme
				dprintf("Khong the dua ra quyet dinh!\n");
				_tree.node_name = "?";
				return;
			}
			for (int i = 0; i < pos; i++, it++)
				;

			// Chon output co so lan xuat hien nhieu nhat lam output chinh cho node la
			dprintf("Quyet dinh node la [%X]: %s\n", &_tree, (*it).data());
			_tree.node_name = *it;

			return;
		}

		// tinh do tinh khiet H_purity
		std::vector<float> *H_min = &(H_purity[0]);

		// bo qua tinh do tinh khiet khi chi con 2 cot (1 cot input - 1 cot output)
		int s = 0;
		if (ROWS <= 2 && outputs.size() <= 1)
			goto pick_smallest_H;
		for (int i = 0; i < ROWS - 1; ++i)
		{
			dprintf("- Tinh H cho %s: \n", _dtlist.names[i].data());
			std::set<std::string>::iterator vit = VALUES[i].begin();
			for (int j = 0; j < VALUES[i].size(); j++)
			{
				dprintf("\tTinh H cho %s=%s: \n", _dtlist.names[i].data(), (*vit).data());
				float h = 0;
				dprintf("\t\t* H(%s=%s) = sum(", _dtlist.names[i].data(), (*vit).data());
				for (int k = 0; k < outputs.size(); k++)
				{
					float r = float(decision_count[s + j][k]) / (occurences_count[s + j]);
					if (r != 0)
						h -= float(r) * (float(log(r)) / log(2));
					// 0*log(0) == 0
					dprintf("[(%d/%d = %.2f) *(%.2f/log(2)) = %.2f] ", decision_count[s + j][k], occurences_count[s + j], r, (r == 0) ? 0 : float(log(r)), (r == 0) ? 0 : -float(r) * (float(log(r)) / log(outputs.size())));
				}
				dprintf(") = %.2f\n", h);
				H_purity[i].push_back(h);

				vit++;
			}

			float H = 0;
			dprintf("\t* H(%s) = sum(", _dtlist.names[i].data());
			for (std::vector<float>::iterator it = H_purity[i].begin(); it != H_purity[i].end(); it++)
			{
				float R = (float(occurences_count[i + (it - H_purity[i].begin())]) / _dtlist.values.size()) * (*it);
				dprintf("[ %d/%d * %.2f = %.2f] ", occurences_count[i + (it - H_purity[i].begin())], _dtlist.values.size(), (*it), R);
				H += R;
			}
			dprintf(") = %.2f\n", H);
			H_purity[i].push_back(H);

			if (H_purity[i].back() < H_min->back())
			{
				H_min = &(H_purity[i]);
				pos_min = i;
			}
			s+=VALUES[i].size();
		}

		// chon cot co gia tri van duc thap hon lam node
		dprintf("[%X] H(%s) co gia tri nho nhat (%f), chon lam gia tri cho ten node\n", &_tree, _dtlist.names[pos_min].data(), H_min->back());
	}
pick_smallest_H:
	_tree.node_name = _dtlist.names[pos_min];
	std::set<std::string> tree_vals = _dtlist.diff_data(_tree.node_name);
	for (std::set<std::string>::iterator it = tree_vals.begin(); it != tree_vals.end(); it++)
	{
		struct linked_tree lk_tree;
		lk_tree.value = (*it);
		lk_tree.linked = new struct tree;
		dprintf("Xay dung cay con voi node %s = %s\n", _tree.node_name.data(), (*it).data());

		// goi ham de quy tim decision tree con voi tap dataset da loc ra
		build_decision_tree(*(lk_tree.linked), _dtlist.loc_list(_tree.node_name, (*it)));
		_tree.node_value.push_back(lk_tree);
	}
}

int main(int argc, char *argv[])
{
	struct dataset_list dataset_cars;
	tree _our_decision_tree;

	dataset_cars.clear();
	dataset_cars.read_file("./dataset_cars.txt", true);

	build_decision_tree(_our_decision_tree, dataset_cars);
	_our_decision_tree.print_tree(0, DEBUG);
	return 0;
}

