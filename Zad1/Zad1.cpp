#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

/* Magic numbers */
#define BUF_SIZE (100)
#define NAME_SIZE (20)
#define MAX_POINTS (50)

/* Custom errors */
#define FILE_ERROR (1)

typedef struct student {
	char name[NAME_SIZE];
	char surname[NAME_SIZE];
	int score;
} student_t;

int countStudents(char const* const fileName)
{
	char* buf = NULL;
	FILE* fp = NULL;
	int ret = 0;
	errno_t err = 0;

	buf = (char*)calloc(BUF_SIZE, sizeof(*buf));
	if (!buf) {
		perror("countStudents: greska pri alokaciji");
		free(buf);
		return err;
	}

	err = fopen_s(&fp, fileName, "r");
	if (err) {
		perror("Greska pri otvaranju datoteke");
		return err;
	}

	while (!feof(fp)) {
		fgets(buf, BUF_SIZE, fp);
		ret++;
	}

	free(buf);
	fclose(fp);
	return ret;
}

int loadAndPrintStudents(char const * const fileName, int const count, student_t* const students)
{
	FILE* fp = NULL;
	errno_t err = 0;

	err = fopen_s(&fp, fileName, "r");
	if (err) {
		perror("loadAndPrintStudents: Greska pri otvaranju datoteke");
		return err;
	}

	for (int i = 0; i < count; i++) {
		int numColumns = fscanf_s(fp, "%s %s %d",
			students[i].name, NAME_SIZE, students[i].surname, NAME_SIZE, &students[i].score);
		if (numColumns < 3) {
			return FILE_ERROR;
		}

		printf("Student: %s %s, %d bodova (%d%%)\n", students[i].surname, students[i].name, students[i].score, students[i].score * 100 / MAX_POINTS);
	}

	fclose(fp);
	return 0;
}

int main()
{
	student_t* students = NULL;
	int count = 0;
	errno_t err = 0;

	count = countStudents("popis.txt");
	if (count < 0) {
		return count;
	}

	students = (student_t*)calloc(count, sizeof(*students));
	if (!students) {
		perror("main: Greska pri alokaciji");
		return errno;
	}

	switch (loadAndPrintStudents("popis.txt", count, students)) {
	case 0:
		break;
	case FILE_ERROR:
		printf("%s: Greska pri ucitavanju studenata %d\n", __func__, FILE_ERROR);
		break;
	default:
		perror("main: Sistemska greska pri ucitavanju studenata");
	}

	free(students);
	return 0;
}