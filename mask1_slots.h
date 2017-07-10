//###############################################################
//# mask1_slots.h for ProcessViewServer created: Mon Jul 3 11:30:12 2017
//# please fill out these slots
//# here you find all possible events
//# Yours: Lehrig Software Engineering
//###############################################################

// todo: uncomment me if you want to use this data aquisiton
// also uncomment this classes in main.cpp and pvapp.h
// also remember to uncomment rllib in the project file
extern rlModbusClient     modbus;  //Change if applicable
extern rlDataAcquisition *acqui;
extern qtDatabase db;
extern rlMutex    dbmutex;
int j=0;
static int counter(int offset)
{
  return modbus.readByte(offset,1)*256 + modbus.readByte(offset,0);
}

typedef struct // (todo: define your data structure here)
{
  char s[4];
}
DATA;

static int slotInit(PARAM *p, DATA *d)
{
  if(p == NULL || d == NULL) return -1;
  //memset(d,0,sizeof(DATA));
  int ret;
  printf("init start\n");
  d->s[0]=-1;
  d->s[1]=-1;
  d->s[2]=-1;
  d->s[3]=-1;
  pvTablePrintf(p,table, -1, 2, "Temperature Up");
  pvTablePrintf(p,table, -1, 3, "Temperature Down");
  pvTablePrintf(p,table, -1, 1, "Humidity Down");
  pvTablePrintf(p,table, -1, 0, "Light up");
  //pvTablePrintf(p,table, 0,-1, "Lamp");
  //pvTablePrintf(p,table, 1,-1, "Switch");
  //pvSetTablePixmap(p,table, 0, 0, "on.png");
  //pvSetTablePixmap(p,table, 0, 1, "on.png");
  //pvSetTablePixmap(p,table, 0, 2, "on.png");
  //pvSetTablePixmap(p,table, 0, 3, "on.png");
  pvSetTableCheckBox(p, table, 0, 0, 0, "S1");
  pvSetTableCheckBox(p, table, 0, 1, 0, "S1");
  pvSetTableCheckBox(p, table, 0, 2, 0, "S1");
  pvSetTableCheckBox(p, table, 0, 3, 0, "S1");
  printf("init stop/n");

  dbmutex.lock();
  ret = db.query(p,"select * from test order by test . ID desc");
  printf("db.query='select * from tables' ret=%d\n", ret);
  db.populateTable(p,DB);
  dbmutex.unlock();

  return 0;
}

static int slotNullEvent(PARAM *p, DATA *d)
{
  if(p == NULL || d == NULL) return -1;
  j++;
  int i;
  const char* test;
  char val[3];
  char insert_str[]="";
  for(i=0;i<=2;i++){
    val[i] = modbus.readByte(0,i*2+1);
  }
  sprintf(insert_str,"INSERT INTO test(time,Temperature,humidity,Light) VALUES (now(),%d,%d,%d)", val[0], val[2], val[1]);
  if(j==15){
    dbmutex.lock();
      int ret;
      ret = db.query(p,insert_str);
      test = db.recordFieldValue(p,1);
      ret = db.query(p,"select * from test order by test . ID desc");
      printf("db.query='select * from tables' ret=%d\n", ret);
      db.populateTable(p,DB);
      dbmutex.unlock();
      j=0;
    }

  pvPrintf(p,LIFE_COUNTER,    "lifeCounter = %s"    ,&test);
  pvPrintf(p,READ_ERROR, "readErrorCount = %d" ,counter(modbusdaemon_READ_ERROR_COUNT_BASE));
  pvPrintf(p,WRITE_ERROR,"writeErrorCount = %d",counter(modbusdaemon_WRITE_ERROR_COUNT_BASE));

  return 0;
}

static int slotButtonEvent(PARAM *p, int id, DATA *d)
{
  if(p == NULL || id == 0 || d == NULL) return -1;
  return 0;
}

static int slotButtonPressedEvent(PARAM *p, int id, DATA *d)
{
  if(p == NULL || id == 0 || d == NULL) return -1;
  return 0;
}

static int slotButtonReleasedEvent(PARAM *p, int id, DATA *d)
{
  if(p == NULL || id == 0 || d == NULL) return -1;
  return 0;
}

static int slotTextEvent(PARAM *p, int id, DATA *d, const char *text)
{
  if(p == NULL || id == 0 || d == NULL || text == NULL) return -1;
  return 0;
}

static int slotSliderEvent(PARAM *p, int id, DATA *d, int val)
{
  if(p == NULL || id == 0 || d == NULL || val < -1000) return -1;
  return 0;
}

static int slotCheckboxEvent(PARAM *p, int id, DATA *d, const char *text)
{
  if(p == NULL || id == 0 || d == NULL || text == NULL) return -1;
  return 0;
}

static int slotRadioButtonEvent(PARAM *p, int id, DATA *d, const char *text)
{
  if(p == NULL || id == 0 || d == NULL || text == NULL) return -1;
  return 0;
}

static int slotGlInitializeEvent(PARAM *p, int id, DATA *d)
{
  if(p == NULL || id == 0 || d == NULL) return -1;
  return 0;
}

static int slotGlPaintEvent(PARAM *p, int id, DATA *d)
{
  if(p == NULL || id == 0 || d == NULL) return -1;
  return 0;
}

static int slotGlResizeEvent(PARAM *p, int id, DATA *d, int width, int height)
{
  if(p == NULL || id == 0 || d == NULL || width < 0 || height < 0) return -1;
  return 0;
}

static int slotGlIdleEvent(PARAM *p, int id, DATA *d)
{
  if(p == NULL || id == 0 || d == NULL) return -1;
  return 0;
}

static int slotTabEvent(PARAM *p, int id, DATA *d, int val)
{
  if(p == NULL || id == 0 || d == NULL || val < -1000) return -1;
  return 0;
}

static int slotTableTextEvent(PARAM *p, int id, DATA *d, int x, int y, const char *text)
{
  if(p == NULL || id == 0 || d == NULL || x < -1000 || y < -1000 || text == NULL) return -1;
  int write_val;
  sscanf(text,"%d",&write_val);
  sscanf(text,"%d,%d",&x,&y);
  if(y==0){
    modbus.writePresetSingleRegister(1,42104,write_val);
  }else if(y==1){
    modbus.writePresetSingleRegister(1,42105,write_val);
  }else if(y==2){
    modbus.writePresetSingleRegister(1,42106,write_val);
  }else if(y==3){
    modbus.writePresetSingleRegister(1,42107,write_val);
  }
  y=0;
  return 0;
}

static int slotTableClickedEvent(PARAM *p, int id, DATA *d, int x, int y, int button)
{
  if(p == NULL || id == 0 || d == NULL || x < -1000 || y < -1000 || button < 0) return -1;
  return 0;
}

static int slotSelectionEvent(PARAM *p, int id, DATA *d, int val, const char *text)
{
  if(p == NULL || id == 0 || d == NULL || val < -1000 || text == NULL) return -1;
  return 0;
}

static int slotClipboardEvent(PARAM *p, int id, DATA *d, int val)
{
  if(p == NULL || id == -1 || d == NULL || val < -1000) return -1;
  return 0;
}

static int slotRightMouseEvent(PARAM *p, int id, DATA *d, const char *text)
{
  if(p == NULL || id == 0 || d == NULL || text == NULL) return -1;
  //pvPopupMenu(p,-1,"Menu1,Menu2,,Menu3");
  return 0;
}

static int slotKeyboardEvent(PARAM *p, int id, DATA *d, int val, int modifier)
{
  if(p == NULL || id == 0 || d == NULL || val < -1000 || modifier < -1000) return -1;
  return 0;
}

static int slotMouseMovedEvent(PARAM *p, int id, DATA *d, float x, float y)
{
  if(p == NULL || id == 0 || d == NULL || x < -1000 || y < -1000) return -1;
  return 0;
}

static int slotMousePressedEvent(PARAM *p, int id, DATA *d, float x, float y)
{
  if(p == NULL || id == 0 || d == NULL || x < -1000 || y < -1000) return -1;
  return 0;
}

static int slotMouseReleasedEvent(PARAM *p, int id, DATA *d, float x, float y)
{
  if(p == NULL || id == 0 || d == NULL || x < -1000 || y < -1000) return -1;
  return 0;
}

static int slotMouseOverEvent(PARAM *p, int id, DATA *d, int enter)
{
  if(p == NULL || id == 0 || d == NULL || enter < -1000) return -1;
  return 0;
}

static int slotUserEvent(PARAM *p, int id, DATA *d, const char *text)
{
  if(p == NULL || id == 0 || d == NULL || text == NULL) return -1;
  return 0;
}
