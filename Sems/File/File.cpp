#include <cstdio>
#include <string>

class File {
    private:
        mutable FILE *file;
        std::string file_name;
        bool opened_for_writing, opened_for_reading;

        void open_file() {
            if (!opened_for_writing) {
                file = fopen(file_name.c_str(), "r");
                opened_for_reading = true;
                opened_for_writing = false;
            } else {
                file = fopen(file_name.c_str(), "w");
                opened_for_reading = false;
                opened_for_writing = true;                
            }
        }

    public:
        File(const std::string &_file_name, bool for_writing = false):
            file(NULL), file_name(_file_name), opened_for_writing(for_writing), opened_for_reading(false) {
            open_file();
        }

        File(const File &t):
            file(NULL), file_name(t.file_name),
            opened_for_writing(t.opened_for_writing),
            opened_for_reading(t.opened_for_reading) {
            open_file();
        }

        ~File() {
            fclose(file);
        }

        char *read(char *buf, int size) {
            if (!opened_for_reading)
                return NULL;
            return fgets(buf, size, file);
        }

        char *write(char *buf, int size) {
            if (!opened_for_writing)
                return NULL;
            opened_for_reading = true;
            for (int i = 0; i < size; i++)
                fputc(buf[i], file);
        }

        File &operator=(const File &t) {
            if (file)
                fclose(file);
            opened_for_writing = t.opened_for_writing;
            opened_for_reading = t.opened_for_reading;
            file_name = t.file_name;
            open_file();
            return *this;
        }
};
/*
int main() {
    char buf[100];
    File f("./myfile");
    f.read(buf, 100);

    File g("./ourfile");
    g.write(buf, 100); // Hmmm?
}                */