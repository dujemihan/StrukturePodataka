#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 100
#define NAME_SIZE 20
#define MAX_POINTS 50

typedef struct student {
	char ime[NAME_SIZE];
	char prezime[NAME_SIZE];
	int bodovi;
} student_t;

int count_students(FILE* fp)
{
	char* buf = (char*) calloc(BUF_SIZE, sizeof(*buf));
	int ret = 0;

	if (!buf) {
		free(buf);
		return -1;
	}

	errno_t err = fseek(fp, 0, SEEK_SET);
	if (err) {
		perror("Nisam se uspio vratiti na pocetak");
		return err;
	}

	while (!feof(fp)) {
		fgets(buf, BUF_SIZE, fp);
		ret++;
	}

	free(buf);
	return ret;
}

int load_print_students(FILE* fp, int n, student_t **ret)
{
	student_t *studenti = (student_t*) calloc(n, sizeof(*studenti));
	errno_t err;

	if (!studenti) {
		perror("Greska pri alokaciji memorije");
		free(studenti);
		return -1;
	}

	err = fseek(fp, 0, SEEK_SET);
	if (err) {
		perror("Nisam se uspio vratiti na pocetak");
		return err;
	}

	for (int i = 0; i < n; i++) {
		int c = fscanf_s(fp, "%s %s %d",
			studenti[i].ime, NAME_SIZE, studenti[i].prezime, NAME_SIZE, &studenti[i].bodovi);
		if (c < 3) {
			printf("Greska u datoteci!\n");
			return -1;
		}

		printf("Student: %s %s, %d bodova (%d%%)\n", studenti[i].prezime, studenti[i].ime, studenti[i].bodovi, studenti[i].bodovi * 100 / MAX_POINTS);
	}

	*ret = studenti;
	return 0;
}

int main()
{
	student_t* studenti;
	FILE* fp;
	int n;

	errno_t err = fopen_s(&fp, "popis.txt", "r");
	if (err) {
		perror("Nisam uspio otvoriti datoteku");
		return err;
	}

	n = count_students(fp);
	if (n < 0) {
		return n;
	}

	err = load_print_students(fp, n, &studenti);
	if (err) {
		return err;
	}

	free(studenti);
	return 0;
}