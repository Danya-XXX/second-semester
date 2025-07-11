void print_Student(Student s) {
	printf("ID: %d\n", s.id);
	printf("���: %s\n", s.fullname);
	printf("�����������: %s\n", s.specialty);
	printf("������� ��������: %s\n", s.level_of_education);
	if (!strcmp(s.level_of_education, "�����������"))
		printf("������� ������������: %s\n", s.info.name);
	else
		printf("����: %d\n", s.info.course);
	printf("������� ����: %.2f\n", s.average_grade);
	printf("--------------\n");
}
void printTree(bin_Tree* node) {
	if (node == NULL)
		return;

	printTree(node->left);
	print_Student(node->data);
	printTree(node->right);
}
void free_tree(bin_Tree* node) {
	if (node) {
		free_tree(node->left);
		free_tree(node->right);
		free(node);
	}
}
int height(bin_Tree* x) {
	return x ? x->height : 0;
}
//������� ������� �� ������ (H_left-H_right)
int balance(bin_Tree* x) {
	if (!x) { return 0; }
	return height(x->left) - height(x->right);
}
bin_Tree* left(bin_Tree* B) {
	if (B == NULL || B->right == NULL) {
		return B; // ������ �� ������������
	}
	bin_Tree* A = B->right;
	bin_Tree* temp1 = A->left;
	A->left = B;
	B->right = temp1;
	A->height = max(height(A->left), height(A->right)) + 1;
	B->height = max(height(B->right), height(B->left)) + 1;
	return A;
}
//������ �������
bin_Tree* right(bin_Tree* B) {
	bin_Tree* A = B->left;
	bin_Tree* temp2 = A->right;
	A->right = B;
	B->left = temp2;
	A->height = max(height(A->left), height(A->right)) + 1;
	B->height = max(height(B->right), height(B->left)) + 1;
	return A;
}


//������� �������� �������� �� id
bin_Tree* del_student(bin_Tree* node, int id) {

	if (node == NULL) {
		printf("�������  �� ������\n");
		return NULL;
	}

	//����� ����(���� id ������ ����� - ���� �����, ����� ������)

	if (id < node->data.id) {
		node->left = del_student(node->left, id);
	}
	else if (id > node->data.id) {
		node->right = del_student(node->right, id);
	}
	else {
		//����� ���������� ���� ������������� 3 ������: ��� ��������, 1 �������, 2 �������
		if (node->left == NULL || node->right == NULL) {
			bin_Tree* temp = node->left ? node->left : node->right;

			// ��� ��������
			if (temp == NULL) {
				temp = node;
				node = NULL;
			}
			// ���� �������
			else {
				*node = *temp; // �������� ���������� �������
			}

			free(temp);
		}

		else {
			//��� �������

			// ���� ����������� ���� 
			bin_Tree* temp = node->right;
			while (temp && temp->left != NULL) {
				temp = temp->left;
			}

			// �������� ������ �� ������������ ���� � ������� ����
			node->data = temp->data;

			// ������� ����������� ����
			node->right = del_student(node->right, temp->data.id);

		}
	}

	// ���� ������ ����� ������
	if (node == NULL) {
		return NULL;
	}

	//���������� ������
	node->height = 1 + max(height(node->left), height(node->right));

	// �������� �������
	int Balance = balance(node);

	//��������������

	// LL
	if (Balance > 1 && balance(node->left) >= 0) {
		return right(node);
	}

	// LR
	if (Balance > 1 && balance(node->left) < 0) {
		node->left = left(node->left);
		return right(node);
	}

	// RR
	if (Balance < -1 && balance(node->right) <= 0) {
		return left(node);
	}

	// RL
	if (Balance < -1 && balance(node->right) > 0) {
		node->right = right(node->right);
		return left(node);
	}

	return node;
}


//���������� ������ ��������
bin_Tree* insert(bin_Tree* node, Student data) {

	//�������� ������ ����
	if (node == NULL) {
		bin_Tree* New = (bin_Tree*)malloc(sizeof(bin_Tree));
		New->left = NULL;
		New->right = NULL;
		New->data = data;
		New->height = 1;
		
		return New;
	}
	//���� id ����������� �������� ������ id �������� � ������� ����, ���� �����, ����� ������. ���� id �����, �� �� ��������� �� ������ id
	if (data.id < node->data.id) {

		node->left = insert(node->left, data);
	}
	else if (data.id > node->data.id) {

		node->right = insert(node->right, data);
	}
	else {
		printf("��� ���� ������������ � ����� id");
		return node;  
	}

	
	node->height = 1 + max(height(node->left), height(node->right));

	//��������� ������
	int Balance = balance(node);

	//�����������

	//LL
	if (Balance > 1 && data.id < node->left->data.id) {
		return right(node);
	}
	//RR
	if (Balance <-1 && data.id > node->right->data.id) {
		return left(node);
	}
	//LR
	if (Balance > 1 && data.id > node->left->data.id) {
		node->left = left(node->left);
		return right(node);
	}
	//RL
	if (Balance < -1 && data.id < node->right->data.id) {
		node->right = right(node->right);
		return left(node);
	}

	return node;
}


//����������� ������� ���������� ��������� � ����
void savestudent(FILE* file, bin_Tree* node) {
	//���� ����� �� �������������� ����, ������������
	if (node == NULL) return;

	//���������� ���� ����� � ����� �������� ������������, ���������� ��������
	savestudent(file, node->left);

	// ������ ������ ��������
	fprintf(file, "%d,%s,%s,%s,", node->data.id, node->data.fullname, node->data.specialty, node->data.level_of_education);


	// ������ ������ � ����������� �� ������� ��������
	if (strcmp(node->data.level_of_education, "�����������") == 0) {
		fprintf(file, "%s,%.2f\n", node->data.info.name, node->data.average_grade);
	}
	else {
		fprintf(file, "%d,%.2f\n", node->data.info.course, node->data.average_grade);
	}
	//���������� ���� � ������ �����
	savestudent(file, node->right);
}



bin_Tree* find_studentID(bin_Tree* x, int id) {
	if (x == NULL) {
		printf("\n������������ �� ������\n\n");
		return NULL;
	}
	if (x->data.id == id) {
		print_Student(x->data);
	}
	else if (x->data.id > id) {
		return find_studentID(x->left, id);
	}
	else if (x->data.id < id) {
		return find_studentID(x->right, id);
	}
}