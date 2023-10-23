class Sudoku
{
	public:
		Sudoku();

		int  random();
		int generate_number_not(int not_num_1, int not_num_2, int not_num_3, int not_num_4);
		void display();
		void input(int x, int y, int value);
		bool check();

	private:
		struct board
		{
			int block_1[2][2]; int block_2[2][2];
			int block_3[2][2]; int block_4[2][2];
		} grid;

		struct actual_values
		{
			int block_1_v[2][2]; int block_2_v[2][2];
			int block_3_v[2][2]; int block_4_v[2][2];
		} values;
};