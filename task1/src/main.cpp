#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <windows.h>
#include "structs_constants.h"
using namespace std;

// TASK A
void enter_base_structures_from_file(ifstream& in, Base base[], int& count){
    count = 0;

    while (in >> base[count].surname >> base[count].name >> base[count].patronymic
        >> base[count].birth
        >> base[count].education >> base[count].marital_status 
        >> base[count].count_children
        >> base[count].profession
        >> base[count].address.city >> base[count].address.street
        >> base[count].address.house >> base[count].address.apartment
        >> base[count].phone ) {
             if (count >= MAX_COUNT_STRUCT - 1) {
                 cerr << "Maximum number of structures" << endl;
                 return;
             }
             count++;
          }
    
    return;
} 
void enter_city_structures_from_file(ifstream& in, City city[], int& count){
    count = 0;

    while (in >> city[count].city >> city[count].code) {
             if (count >= MAX_COUNT_STRUCT - 1) {
                 cerr << "Maximum number of structures." << endl;
                 return;
             }
             count++;
          }
    return;
}
bool is_first_half(const char* birth) {
    char day_str[3] = {};
    if (strlen(birth) < 2) return false; 
    
    strncpy(day_str, birth, 2);
    day_str[2] = '\0';
    
    int day = atoi(day_str);
    return (day >= 1 && day <= 15);
}

void task_a(Base base[], int count_base, City city[], int count_city, 
    ofstream& out_first_half_bin, ofstream& out_second_half_bin) {
    Result_a result;
    
    for (int i = 0; i < count_base; i++) {
        int city_code = -1;
        bool city_found = false;
        
        for (int j = 0; j < count_city; j++) {
            if (strcmp(base[i].address.city, city[j].city) == 0) {
                city_code = city[j].code;
                city_found = true;
                break;
            }
        }
        
        if (city_found) {
            strcpy_s(result.surname, sizeof(result.surname), base[i].surname);
            strcpy_s(result.name, sizeof(result.name), base[i].name);
            strcpy_s(result.patronymic, sizeof(result.patronymic), base[i].patronymic);
            strcpy_s(result.birth, sizeof(result.birth), base[i].birth);
            
            result.address = base[i].address;
            result.phone = base[i].phone;
            result.code = city_code;
            
            if (is_first_half(result.birth)) {
                out_first_half_bin.write((char*)&result, sizeof(Result_a));
            } else {
                out_second_half_bin.write((char*)&result, sizeof(Result_a));
            }
        }
    }
}

void bin_to_txt_a (ifstream& in_bin, ofstream& out_txt) {
    Result_a result;

    while (in_bin.read((char*)&result, sizeof(Result_a))) {
        out_txt << result.surname << " "
                << result.name << " "
                << result.patronymic << " "
                << result.birth << " "
                << result.address.city << " "
                << result.address.street << " "
                << result.address.house << " "
                << result.address.apartment << " "
                << result.phone << " "
                << result.code << endl;
    }
}
void enter_result_structures_from_file(ifstream& in, Result_a result[], int& count) {
    count = 0;
    Result_a temp_result;

    while (in.read(reinterpret_cast<char*>(&temp_result), sizeof(Result_a))) {
        if (count >= MAX_COUNT_STRUCT) {
            cerr << "Maximum number of structures" << endl;
            return;
        }
        
        result[count] = temp_result;
        count++;
    }

    if (!in.eof() && count < MAX_COUNT_STRUCT) {
          cerr << "Error reading data from binary file" << endl;
    }
}

// TASK B
const int W1 = 30;
const int W2 = 8;
const int W3 = 6;
const int W4 = 6;
const int INTERNAL_DATA_LEN = W1 + W2 + W3 + W4 + 3 * 3;
const int INTERNAL_COL_WIDTH = 1 + INTERNAL_DATA_LEN + 1;
const int TABLE_WIDTH = 1 + INTERNAL_COL_WIDTH + 1 + 1 + INTERNAL_COL_WIDTH + 1; 

size_t get_visual_length(const char* str) {
    size_t len = 0;
    size_t byte_count = strlen(str);

    for (size_t i = 0; i < byte_count; ++i) {
        if ((unsigned char)str[i] >= 0xC0) {
            len++;
            i++; 
        } else {
            len++;
        }
    }
    return len;
}

void print_spaces(ofstream& out, int count) {
    for (int i = 0; i < count; ++i) {
        out << ' ';
    }
}
void print_padded_str(ofstream& out, const char* str, int width) {
    out << str;
    int visual_len = (int)get_visual_length(str); 

    if (visual_len < width) {
        print_spaces(out, width - visual_len);
    }
}
void print_padded_num(ofstream& out, int number, int width) {
    char buffer[20];
    snprintf(buffer, 20, "%d", number);
    print_padded_str(out, buffer, width);
}
int extract_month_int(const char* birth) {
    char month_str[3] = {};

    strncpy(month_str, birth + 2, 2);
    month_str[2] = '\0';

    return atoi(month_str);
}
void print_person_info(ofstream& out, const Result_a& person) {
    char fio_buffer[W1 + 1];
    
    char str_name[4] = "";
    if (strlen(person.name) > 1) {
        str_name[0] = person.name[0]; 
        str_name[1] = person.name[1]; 
        str_name[2] = '.'; 
        str_name[3] = '\0';
    }
    char str_patronymic[4] = "";
    if (strlen(person.patronymic) > 1) {
        str_patronymic[0] = person.patronymic[0]; 
        str_patronymic[1] = person.patronymic[1]; 
        str_patronymic[2] = '.'; 
        str_patronymic[3] = '\0';
    }

    snprintf(fio_buffer, W1 + 1, "%s %s%s",
             person.surname, str_name, str_patronymic);
    print_padded_str(out, fio_buffer, W1);

    out << " | ";
    print_padded_str(out, person.birth, W2);

    out << " | ";
    print_padded_num(out, person.phone, W3);

    out << " | ";
    print_padded_num(out, person.code, W4);
}

void task_b(Result_a first_half[], int count_first_half, Result_a second_half[], int count_second_half, ofstream& out) {
    setlocale(LC_ALL, "ru");

    const char* month_names[] = {
        "ЯНВАРЬ", "ФЕВРАЛЬ", "МАРТ", "АПРЕЛЬ", "МАЙ", "ИЮНЬ",
        "ИЮЛЬ", "АВГУСТ", "СЕНТЯБРЬ", "ОКТЯБРЬ", "НОЯБРЬ", "ДЕКАБРЬ"
    };

    const int EMPTY_SPACE_WIDTH = INTERNAL_DATA_LEN; 

    for (int month = 1; month <= 12; month++) {
        int first_indices[MAX_COUNT_STRUCT];
        int second_indices[MAX_COUNT_STRUCT];
        int first_count = 0;
        int second_count = 0;

        for (int k = 0; k < count_first_half; k++) {
            if (extract_month_int(first_half[k].birth) == month) {
                first_indices[first_count++] = k;
            }
        }
        for (int k = 0; k < count_second_half; k++) {
            if (extract_month_int(second_half[k].birth) == month) {
                second_indices[second_count++] = k;
            }
        }

        int max_lines_for_month = (first_count > second_count) ? first_count : second_count;
        if (max_lines_for_month == 0) {
            continue;
        }

        out << "|";
        int month_name_visual_len = (int)get_visual_length(month_names[month]);
        int padding_total = TABLE_WIDTH - 2 - month_name_visual_len; 
        int padding_left = padding_total / 2;
        int padding_right = padding_total - padding_left - 1;

        for (int i = 0; i < padding_left; i++) out << '-';
        out << month_names[month];
        for (int i = 0; i < padding_right; i++) out << '-';
        out << "|" << endl;

        out << "|";
        for (int i = 0; i < INTERNAL_COL_WIDTH; i++) out << '-'; 

        out << "+"; 
        
        for (int i = 0; i < INTERNAL_COL_WIDTH; i++) out << '-';
        out << "|" << endl;


        for (int i = 0; i < max_lines_for_month; i++) {
            out << "| ";

            if (i < first_count) {
                int person_index = first_indices[i];
                print_person_info(out, first_half[person_index]);
            } else {
                print_spaces(out, EMPTY_SPACE_WIDTH); 
            }

            out << " | ";
            
            if (i < second_count) {
                int person_index = second_indices[i];
                print_person_info(out, second_half[person_index]);
            } else {
                print_spaces(out, EMPTY_SPACE_WIDTH); 
            }
            
            out << " |" << endl;
        }

        out << "|";
        for (int i = 0; i < INTERNAL_COL_WIDTH; i++) out << '=';
        out << "="; 
        for (int i = 0; i < INTERNAL_COL_WIDTH; i++) out << '=';
        out << "|" << endl;
    }
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    SetConsoleOutputCP(CP_UTF8);

    /*
    ifstream in_base("../input/base.txt");
    ifstream in_city("../input/city.txt");
    if (!in_base.is_open() || !in_city.is_open()) {
        cerr << "Error: Failed to open input file base.txt or city.txt" << endl;
        return 1;
    }
    
    Base base[MAX_COUNT_STRUCT];
    int count_base_structures = 0;
    enter_base_structures_from_file(in_base, base, count_base_structures);
    in_base.close();
    
    City city[MAX_COUNT_STRUCT];
    int count_city_structures = 0;
    enter_city_structures_from_file(in_city, city, count_city_structures);
    in_city.close();
    
    if (count_base_structures == 0) {
        cerr << "Failed to load records from base.txt" << endl;
        return 1;
    }

    ofstream out_first_half_bin("../output/out_first_half.bin", ios::binary);
    ofstream out_second_half_bin("../output/out_second_half.bin", ios::binary);
    if (!out_first_half_bin.is_open() || !out_second_half_bin.is_open()) {
       cerr << "Error: Failed to open output binary files" << endl;
       return 1;
    }
    
    task_a(base, count_base_structures, city, count_city_structures, 
           out_first_half_bin, out_second_half_bin);
    out_first_half_bin.close();
    out_second_half_bin.close();
    */

    ifstream in_first_half_bin ("../output/out_first_half.bin", ios::binary), 
    in_second_half_bin ("../output/out_second_half.bin", ios::binary);
    if (!in_first_half_bin.is_open() || !in_second_half_bin.is_open()) {
         cerr << "Error: Failed to open input file" << endl;
         return 1;
    }

    Result_a fisrt_half [MAX_COUNT_STRUCT], second_half [MAX_COUNT_STRUCT];
    int count_fisrt_half = 0, count_second_half = 0;
    enter_result_structures_from_file(in_first_half_bin, fisrt_half, count_fisrt_half);
    in_first_half_bin.close();
    enter_result_structures_from_file(in_second_half_bin, second_half, count_second_half);
    in_second_half_bin.close();
    
    if (count_fisrt_half == 0 && count_second_half == 0) {
        cerr << "Error: No data loaded from binary files" << endl;
        return 1;
    }

    ofstream out_b ("../output/out_table.txt");
    if (!out_b.is_open()) {
         cerr << "Error: Failed to open output file: ../output/out_table.txt" << endl;
         return 1;
    }
    
    task_b(fisrt_half, count_fisrt_half, second_half, count_second_half, out_b);
    out_b.close();

    return 0;
}
