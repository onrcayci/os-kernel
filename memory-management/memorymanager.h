char *fileName;

int launcher(FILE *p);

int countTotalPages(FILE *f);

void loadPage(int pageNumber, FILE *f, int frameNumber);

int findFrame();

int findVictim(struct PCB *p);

int updatePageTable(struct PCB *p, int pageNumber, int frameNumber, int victimFrame);