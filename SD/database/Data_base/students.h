enum Status {
	insert_student = 1,
	find_Student,
	delete_student,
	add_data,
	Exit,
	print_studentsTree
};
typedef union {
	int course;
	char name[100];
}Level;


typedef struct {
	int id;
	char fullname[100];
	char specialty[100];
	char level_of_education[15];
	Level info;
	float average_grade;
} Student;

typedef struct bin_Tree {
	int height;
	Student data;
	struct bin_Tree* left;
	struct bin_Tree* right;
}bin_Tree;