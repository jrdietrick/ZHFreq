// ZHFreq.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include "ZiNode.h"

#define CHART_WIDTH 63
#define CHART_HEIGHT 40
#define CHAR_COUNT CHART_WIDTH * CHART_HEIGHT

#define ZI_WIDTH 25
#define ZI_HORIZ_PAD 2
#define ZI_VERT_PAD 2

#define X_ALL_TRANSLATE 0
#define Y_ALL_TRANSLATE 80 - (100 - ZI_WIDTH)
#define X_INDIV_SCALE 1 //1.0131297
#define Y_INDIV_SCALE 1 //0.9870405

#define FREQ_SCALE_AMPLITUDE 2

#define OPACITY_BASE 0.25
#define FREQ_SCALE_OPACITY 0.75

#define COLOR 0

#define BACK_R 0
#define BACK_G 0
#define BACK_B 0

#define BACK_A 1

using namespace std;

ifstream infile;

static double max_percentile, min_percentile;

void print_svg_header(FILE* outf) {
	float w, h;
	w = (float) (ZI_WIDTH * CHART_WIDTH + ZI_HORIZ_PAD * (CHART_WIDTH + 1));
	h = (float) (ZI_WIDTH * CHART_HEIGHT + ZI_VERT_PAD * (CHART_HEIGHT + 1));
	fprintf(outf, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n");
	fprintf(outf, "<svg\n");
    fprintf(outf, "\txmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n");
    fprintf(outf, "\txmlns:cc=\"http://creativecommons.org/ns#\"\n");
    fprintf(outf, "\txmlns:svg=\"http://www.w3.org/2000/svg\"\n");
    fprintf(outf, "\txmlns=\"http://www.w3.org/2000/svg\"\n");
    fprintf(outf, "\twidth=\"%lfpx\"\n", w);
   	fprintf(outf, "\theight=\"%lfpx\">\n", h);
	fprintf(outf, "\t<g><rect x=\"0\" y=\"0\" width=\"%lfpx\" height=\"%lfpx\" style=\"fill:#%02X%02X%02X;fill-opacity:%lf;\" />\n", w, h, (int) BACK_R, (int) BACK_G, (int) BACK_B, (float) BACK_A);
}

void print_svg_footer(FILE* outf) {
	fprintf(outf, "\t</g>\n</svg>\n");
	/*
	fprintf(outf, "\t<text font-family=\"Unsteady Oversteer\" font-size=\"%dpx\" fill=\"#333333\" x=\"%lf\" y=\"%lf\" transform=\"rotate(90)\">\n", text_height, text_x, text_y);
    fprintf(outf, "\t\t(C)%04d john r dietrick\n", Tm->tm_year+1900);
  	fprintf(outf, "\t</text>\n");
	fprintf(outf, "\t<text font-family=\"Unsteady Oversteer\" font-size=\"%dpx\" fill=\"#333333\" x=\"%lf\" y=\"%lf\" transform=\"rotate(90)\">\n", text_height, text_x, text_y + (text_height * 1.25));
	fprintf(outf, "\t\tscale/stroke: %d/%d\n", SCALE_FACTOR, STROKE_WIDTH);
  	fprintf(outf, "\t</text>\n");
	fprintf(outf, "\t<text font-family=\"Unsteady Oversteer\" font-size=\"%dpx\" fill=\"#333333\" x=\"%lf\" y=\"%lf\" transform=\"rotate(90)\">\n", text_height, text_x, text_y + (text_height * 2 * 1.25));
	fprintf(outf, "\t\tbr init/re: (+1) %04.2f-%04.2f/%04.2f-%04.2f\n", INIT_BR_MIN, INIT_BR_MAX, RE_BR_MIN, RE_BR_MAX);
  	fprintf(outf, "\t</text>\n");
	fprintf(outf, "\t<text font-family=\"Unsteady Oversteer\" font-size=\"%dpx\" fill=\"#333333\" x=\"%lf\" y=\"%lf\" transform=\"rotate(90)\">\n", text_height, text_x, text_y + (text_height * 3 * 1.25));
	fprintf(outf, "\t\tlen segs/range: %d-%d/%d-%d\n", (int) BR_MIN_PARTS, (int) BR_MAX_PARTS, (int) BR_PART_MIN_LENGTH, (int) BR_PART_MAX_LENGTH);
  	fprintf(outf, "\t</text>\n");
	fprintf(outf, "\t<text font-family=\"Unsteady Oversteer\" font-size=\"%dpx\" fill=\"#333333\" x=\"%lf\" y=\"%lf\" transform=\"rotate(90)\">\n", text_height, text_x, text_y + (text_height * 4 * 1.25));
	fprintf(outf, "\t\tang range/var: %04.2f-%04.2f/%04.2f\n", BR_MIN_ANGLE, BR_MAX_ANGLE, BR_MAX_ANGLE_VAR);
  	fprintf(outf, "\t</text>\n");
	fprintf(outf, "\t<text font-family=\"Unsteady Oversteer\" font-size=\"%dpx\" fill=\"#333333\" x=\"%lf\" y=\"%lf\" transform=\"rotate(90)\">\n", text_height, text_x, text_y + (text_height * 5 * 1.25));
	fprintf(outf, "\t\t%04d-%02d-%02d %02d:%02d:%02d\n", Tm->tm_year+1900, Tm->tm_mon+1, Tm->tm_mday, Tm->tm_hour, Tm->tm_min, Tm->tm_sec);
  	fprintf(outf, "\t</text>\n");
	fprintf(outf, "\t</g>\n</svg>\n");
	*/
}

void print_ZiNode(FILE* outf, ZiNode* target, int x_idx, int y_idx) {
	int r, g, b;
	float x, y, size, opacity;
	float amplitude_addend;

	amplitude_addend = (float)(((max_percentile - target->centile) / (max_percentile - min_percentile) * FREQ_SCALE_AMPLITUDE) * ZI_WIDTH);
	x = (float)(x_idx * ZI_WIDTH + ZI_HORIZ_PAD * (x_idx + 1) + X_ALL_TRANSLATE - amplitude_addend / 2);
	y = (float)(y_idx * ZI_WIDTH + ZI_VERT_PAD * (y_idx + 1) + Y_ALL_TRANSLATE + amplitude_addend / 2);
	size = (float)(amplitude_addend + ZI_WIDTH);
	opacity = (float)(OPACITY_BASE + ((max_percentile - target->centile) / (max_percentile - min_percentile) * FREQ_SCALE_OPACITY));

	r = 255;
	g = 255;
	b = 255;

#if (COLOR == 1)
	switch(target->tone) {
		case 1:
			b = 255;
			break;
		case 2:
			g = 128;
			break;
		case 3:
			r = 128;
			break;
		case 4:
			r = 51;
			b = 102;
			break;
		default:
			break;
	}
#endif

	fprintf(outf, "\t<text style=\"font-family:STXinwei;font-size:%lfpx;fill:#%02X%02X%02X;fill-opacity:%lf;\" x=\"%lf\" y=\"%lf\" transform=\"scale(%lf,%lf)\">\n", size, r, g, b, opacity, (float) x, (float) y, (float) X_INDIV_SCALE, (float) Y_INDIV_SCALE);
    fprintf(outf, "\t\t<tspan x=\"%lf\" y=\"%lf\">%c%c%c</tspan>\n", (float) x, (float) y, target->data[0], target->data[1], target->data[2]);
  	fprintf(outf, "\t</text>\n");

}

void parse_freq_file(ZiNode ** syms) {
	int tone = 0;
	int rank, j;
	char databuf[4];
	double raw_freq;
	double centile;
	char fayin[10];

	for(int i = 0; i < CHAR_COUNT; i++) {
		infile >> rank;
		infile.seekg(1, ios::cur);
		infile.get(databuf, 4);
		infile.seekg(1, ios::cur);
		infile >> raw_freq;
		infile.seekg(1, ios::cur);
		infile >> centile;
		infile.seekg(1, ios::cur);
		j = 0;
		while((infile.peek() != '/') && (infile.peek() != ' ') && (infile.peek() != 9) && (j < 9)) {
			char temp = infile.peek();
			infile >> fayin[j];
			j++;
		}
		if((fayin[j - 1] >= '0') && (fayin[j - 1] <= '5')) {
			tone = fayin[j - 1] - 0x30;
			j--;
			fayin[j] = '\0';
		} else {
			fayin[j] = '\0';
		}
		infile.ignore(999, '\n');
		syms[i] = new ZiNode(rank, databuf, raw_freq, centile, fayin, tone);
	}
}

void swap_ZiNode(ZiNode* a, ZiNode* b) {
	int rank;
	char databuf[3];
	double raw_freq;
	double centile;
	char fayin[10];
	int tone;

	rank = a->rank;
	memcpy(databuf, a->data, 3);
	raw_freq = a->raw_freq;
	centile = a->centile;
	memcpy(fayin, a->fayin, 10);
	tone = a->tone;

	a->rank = b->rank;
	memcpy(a->data, b->data, 3);
	a->raw_freq = b->raw_freq;
	a->centile = b->centile;
	memcpy(a->fayin, b->fayin, 10);
	a->tone = b->tone;

	b->rank = rank;
	memcpy(b->data, databuf, 3);
	b->raw_freq = raw_freq;
	b->centile = centile;
	memcpy(b->fayin, fayin, 10);
	b->tone = tone;
}

void sift_down_ZiNode(ZiNode** syms, int index, int count) {
	int root = index;
	while ((root * 2 + 2) <= count) {
		int cmp1, cmp2;
		int child = root * 2 + 1;
		if ((child + 2) < count) {
			cmp1 = strcmp(syms[child]->fayin, syms[child + 1]->fayin);
		} else {
			cmp1 = 0;
		}
		if (((child + 2) < count) && (cmp1 < 0)) {
			child += 1;
		} else if (((child + 2) < count) && (cmp1 == 0) && (syms[child]->tone < syms[child + 1]->tone)) {
			child += 1;
		}
		cmp2 = strcmp(syms[root]->fayin, syms[child]->fayin);
		if (cmp2 < 0) {
			swap_ZiNode(syms[root], syms[child]);
			root = child;
		} else if ((cmp2 == 0) && (syms[root]->tone < syms[child]->tone)) {
			swap_ZiNode(syms[root], syms[child]);
			root = child;
		} else {
			return;
		}
	}
}

void heapify_ZiNode(ZiNode** syms, int count) {
	int start = (count - 2) / 2;
	while (start >= 0) {
		sift_down_ZiNode(syms, start, count);
		start--;
	}
}

void heap_sort_ZiNode(ZiNode** syms, int count) {
	int end = count - 1;
	heapify_ZiNode(syms, count);
	
	while (end > 0) {
		swap_ZiNode(syms[0], syms[end]);
		end--;
		sift_down_ZiNode(syms, 0, end + 1);
	}
}

int main(int argc, char* argv[])
{
	FILE* outf;

	if(argc != 3) {
		printf("Need precisely two arguments!\n");
		return 1;
	}

	ZiNode ** syms = new ZiNode*[CHAR_COUNT];
	
	infile.open(argv[1], ifstream::in | ifstream::binary);
	parse_freq_file(syms);

	max_percentile = syms[CHAR_COUNT - 1]->centile;
	min_percentile = syms[0]->centile;

	heap_sort_ZiNode(syms, CHAR_COUNT);
	infile.close();

	outf = fopen(argv[2], "w+");
	if (outf == NULL) {
		printf("Error creating output file!\n");
		return 1;
	}
	
	print_svg_header(outf);
	for (int i = 0; i < CHART_HEIGHT; i++) {
		for (int j = 0; j < CHART_WIDTH; j++) {
			print_ZiNode(outf, syms[i * CHART_WIDTH + j], j, i);
		}
	}
	print_svg_footer(outf);

	fclose(outf);

	delete[] syms;

	return 0;
}



