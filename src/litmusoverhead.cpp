#include "litmusoverhead.hpp"

LitmusOverhead* LitmusOverhead::litmusOverheadInstance = 0;


LitmusOverhead::LitmusOverhead(){

}

LitmusOverhead* LitmusOverhead::getInstance(){
  if(!litmusOverheadInstance){
    litmusOverheadInstance = new LitmusOverhead();
  }
  return litmusOverheadInstance;
}



int LitmusOverhead::initOverhead(const char* traceBufferName) {

  maxCXS = 0;
  maxSCHED = 0;
  maxSCHED2 = 0;
  maxRELEASE = 0;
  maxRELEASE_LATENCY = 0;
  maxSEND_RESCHED = 0;

  this->traceBufferName = traceBufferName;

  this->traceBufFD = open(this->traceBufferName, O_RDWR);
  if (this->traceBufFD < 0) {
    perror("could not open feathertrace");
    return 1;
  }
  this->eventCount = 0;
  this->nbTraceEvents = 0;
  addEvent("CXS_START");
  addEvent("CXS_END");
  addEvent("SCHED_START");
  addEvent("SCHED_END");
  addEvent("SCHED2_START");
  addEvent("SCHED2_END");
  addEvent("RELEASE_START");
  addEvent("RELEASE_END");
  addEvent("RELEASE_LATENCY");
  addEvent("SEND_RESCHED_START");
  addEvent("SEND_RESCHED_END");


  for (int ec=0;ec<this->nbTraceEvents;ec++) {
    if (!this->enableEvent(this->traceBufFD, this->traceEvent[ec])) {
      fprintf(stderr, "Enabling %s failed: %m\n",this->traceEvent[ec] );
      return 2;
    }	
  }
  this->threshold = CYCLES_PER_US * 10000;
  this->wantBestEffort = 0;
  this->wantInterleaved = 1;
  this->bytesRead = 0;

  this->startTracing(traceBufFD);
}

void LitmusOverhead::addEvent(char* eventID) {
  traceEvent[nbTraceEvents] = eventID;
  nbTraceEvents++ ;
}

void LitmusOverhead::startTracing(int fd)
{
  int rd;
  size_t size, count;
  struct timestamp *ts, *end;

  while ((rd = read(fd, this->traceBuffer, 4096)) > 0) {
    this->bytesRead += rd;
    size = rd;
    /* fwrite(buf, 1, rd, stdout); */
    ts    = (struct timestamp*) this->traceBuffer;
    count = size / sizeof(struct timestamp);
    end   = ts + count;
		
    this->updateMaxOverhead2(ts, end, TS_CXS_START); 
    this->updateMaxOverhead2(ts, end, TS_SCHED_START); 
    this->updateMaxOverhead2(ts, end, TS_SCHED2_START); 
    this->updateMaxOverhead2(ts, end, TS_RELEASE_START); 
    this->updateMaxOverhead2(ts, end, TS_SEND_RESCHED_START); 
    this->updateMaxOverhead(ts, end, TS_RELEASE_LATENCY);
  }
}


void LitmusOverhead::updateMaxOverhead(struct timestamp* start, struct timestamp* end,
				       unsigned long id) {
  
  // printf(" \n Debug maxRELEASE_LATENCY %llu \n", this->maxRELEASE_LATENCY);
  struct timestamp *first;
  
  for (; start != end; start++)
    if (start->event == id){
      first = start;
      if (first)
	if ( !(first->task_type != TSK_RT && first->task_type != TSK_RT && !wantBestEffort) ) {
	  if ((overhead_t)(first->timestamp) > this->maxRELEASE_LATENCY){
	    // printf("current overhead is %llu \n",(overhead_t)(first->timestamp - first->timestamp));
	    switch (id) {
	    case TS_RELEASE_LATENCY:
	      maxRELEASE_LATENCY =  (overhead_t)(first->timestamp);
	      this->updateLitmusOverheadObservers();
	      break;
	    }
	  }
	}
    }
}


void LitmusOverhead::updateMaxOverhead2(struct timestamp* start, struct timestamp* end,
					unsigned long id) {

  // printf(" \n Debug maxCXS %llu \n",this->maxCXS);
  // printf(" \n Debug maxSCHED %llu \n",this->maxSCHED);
  // printf(" \n Debug maxSCHED2 %llu \n",this->maxSCHED2);
  // printf(" \n Debug maxRELEASE %llu \n",this->maxRELEASE);
  // printf(" \n Debug maxRESCHED_SEND %llu \n",this->maxRELEASE);
  struct timestamp *first;
  struct timestamp *second;

  while (start !=end && start->event != id + 1) {
    start++;
  }

  for (; start != end; start++)
    if (start->event == id){
      first = start;
      second = next_id(start + 1, end, start->cpu, start->event + 1,
		       start->event);
      if (second)
	if ( !(second->timestamp - first->timestamp > threshold) ||
	     !(first->task_type != TSK_RT && second->task_type != TSK_RT && !wantBestEffort) ) {
	  // if ((overhead_t)(second->timestamp - first->timestamp) > this->maxCXS){
	  // printf("current overhead is %llu \n",(overhead_t)(second->timestamp - first->timestamp));
	  switch (id) {
	  case TS_CXS_START:
	    if ((overhead_t)(second->timestamp - first->timestamp) > this->maxCXS) {
	      maxCXS =  (unsigned long long)(second->timestamp - first->timestamp);
	      this->updateLitmusOverheadObservers();
	    }
	    break;
	  case TS_SCHED_START:
	    if ((overhead_t)(second->timestamp - first->timestamp) > this->maxSCHED) {
	      maxSCHED =  (unsigned long long)(second->timestamp - first->timestamp);
	      this->updateLitmusOverheadObservers();
	    }
	    break;
	  case TS_SCHED2_START:
	    if ((overhead_t)(second->timestamp - first->timestamp) > this->maxSCHED2) {
	      maxSCHED2 =  (unsigned long long)(second->timestamp - first->timestamp);
	      this->updateLitmusOverheadObservers();
	    }
	    break;
	  case TS_RELEASE_START:
	    if ((overhead_t)(second->timestamp - first->timestamp) > this->maxRELEASE) {
	      maxRELEASE =  (unsigned long long)(second->timestamp - first->timestamp);
	      this->updateLitmusOverheadObservers();
	    }
	    break;
	  case TS_SEND_RESCHED_START:
	    if ((overhead_t)(second->timestamp - first->timestamp) > this->maxSEND_RESCHED) {
	      maxSEND_RESCHED =  (unsigned long long)(second->timestamp - first->timestamp);
	      printf("max send resched reached ");
	      this->updateLitmusOverheadObservers();
	    }
	    break;
	  default:
	    break;
	    
	  }
	}		  
    }
}

struct timestamp* LitmusOverhead::next(struct timestamp* start, struct timestamp* end,
				       int cpu) {
  struct timestamp* pos;
  unsigned int last_seqno = 0;

  for (pos = start; pos != end;  pos++) {
    /* check for for holes in the sequence number */
    if (last_seqno && last_seqno + 1 != pos->seq_no) {
      /* stumbled across a hole */
      return NULL;
    }
    last_seqno = pos->seq_no;

    if (pos->cpu == cpu)
      return pos;
  }
  return NULL;
}


struct timestamp* LitmusOverhead::next_id(struct timestamp* start, struct timestamp* end,
					  int cpu, unsigned long id,
					  unsigned long stop_id) {


  struct timestamp* pos = start;
  int restarts = 0;

  while ((pos = next(pos, end, cpu))) {
    if (pos->event == id)
      break;
    else if (pos->event == stop_id)
      return NULL;
    pos++;
    restarts++;
    if (!this->wantInterleaved)
      return NULL;
  }
  // if (pos)
  // 	interleaved += restarts;
  return pos;
}

void LitmusOverhead::stopTracing()
{
  int ok;
  ok = this->disableAll(this->traceBufFD);
  if (!ok)
    fprintf(stderr, "disable_all: %m\n");
}

int LitmusOverhead::enableEvent(int fd, char* str)
{
  cmd_t   *id;
  int err;

  id = ids + this->eventCount;
  if (!str2event(str, id)) {
    errno = EINVAL;
    return 0;
  }
  this->eventCount += 1;
  err = ioctl(fd, ENABLE_CMD, *id);

  if (err < 0)
    printf("ioctl(%d, %d, %d) => %d (errno: %d)\n", fd, (int) ENABLE_CMD, *id,
	   err, errno);

  return err == 0;
}

int LitmusOverhead::disableAll(int fd)
{

  printf ("debug eventCount = %d \n",this->eventCount);
  printf ("debug maxCXS = %llu \n",this->maxCXS);
  printf ("debug maxSCHED = %llu \n",this->maxSCHED);
  printf("Debug maxSCHED2 %llu \n",this->maxSCHED2);
  printf("Debug maxRELEASE %llu \n",this->maxRELEASE);
  printf("Debug maxSEND_RESCHED %llu \n",this->maxSEND_RESCHED);
  printf("Debug maxRELEASE_LATENCY %llu \n",this->maxRELEASE_LATENCY);


  int disabled = 0;
  int i;
  fprintf(stderr, "Disabling %d events.\n", this->eventCount);
  for (i = 0; i < this->eventCount; i++){
    if (ioctl(fd, DISABLE_CMD, this->ids[i]) < 0)
      perror("ioctl(DISABLE_CMD)");
    else{
      disabled++;
    }
  }
  return  disabled == this->eventCount;
}

void LitmusOverhead::setLitmusOverheadObserver(Overhead* overhead) {
  this->overhead = overhead;
}
void LitmusOverhead::updateLitmusOverheadObservers() {
  this->overhead->updateCXS(this->maxCXS);
  this->overhead->updateSCHED(this->maxSCHED);
  this->overhead->updateSCHED2(this->maxSCHED2);
  this->overhead->updateRELEASE(this->maxRELEASE);
  this->overhead->updateSEND_RESCHED(this->maxSEND_RESCHED);
  this->overhead->updateRELEASE_LATENCY(this->maxRELEASE_LATENCY);
  
}
