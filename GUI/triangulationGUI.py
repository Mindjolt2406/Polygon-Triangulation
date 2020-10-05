import pygame,sys,os
import time
from pygame.locals import *
from random import *
pygame.init()
DISPLAYSURF = pygame.display.set_mode((1000,800))
pygame.display.set_caption("Polygon Triangulation")
# pygame.draw.rect(DISPLAYSURF,(0,0,0),(0,0,100,100))

BLUE = (0,0,255)
LIGHTDARKGREY = (150,150,150)
BLACK = (0,0,0)
WHITE = (255,255,255)

def drawGrid():
  '''This function draws the lines diffrentiating the squares'''
  for i in range(81):
    pygame.draw.line(DISPLAYSURF,(200,200,200),(10*i,0),(10*i,800),2)
    pygame.draw.line(DISPLAYSURF,(200,200,200),(0,10*i),(800,10*i),2)
  pygame.display.update()

def addPoint(mouseX, mouseY,points):
  point = ((mouseX/10)*10, (mouseY/10)*10)
  if(len(points) and point == points[0]): 
    return True
  points.append(point)
  return False

def renderPoints(lastPointDone,points):
  for point in points:
    pygame.draw.circle(DISPLAYSURF, (0,0,128),point,5)

  if(len(points) >= 2):
    for i in range(len(points)-1):
      pygame.draw.line(DISPLAYSURF, BLUE, points[i], points[i+1], 2)

  if(lastPointDone):
    pygame.draw.line(DISPLAYSURF, BLUE, points[0], points[len(points)-1],2)

  pygame.display.update()

def drawMouseHover(mouseX, mouseY, points,notHover = False):
  if(mouseX > 800): return
  if(notHover): 
    return
  if(len(points) == 0):
    # DISPLAYSURF , Colour, Center, Radius
    pygame.draw.circle(DISPLAYSURF, LIGHTDARKGREY, ((mouseX/10)*10, (mouseY/10)*10), 5)

  else:
    # Draw a line from the last point to this point
    lastPoint = points[len(points)-1]
    pygame.draw.line(DISPLAYSURF, (0,0,255), lastPoint , ((mouseX/10)*10, (mouseY/10)*10), 2)
    pygame.draw.circle(DISPLAYSURF, LIGHTDARKGREY, ((mouseX/10)*10, (mouseY/10)*10), 5)
  
  pygame.display.update()


def printPoints(points):
  file = open("formatInput.txt","w")
  lines = []
  lines.append(str(len(points)) + "\n")
  for point in points:
    temp = str(point[0]) + " " + str(point[1]) + "\n"
    lines.append(temp)
  file.writelines(lines)
  file.close()

hasPrinted = False

def restart():
  return [],False, False

def inputPoints():
  points = []
  lastPointDone = False
  hasPrinted = False

  while(True):
    DISPLAYSURF.fill((255,255,255))
    drawGrid()
    mouseX,mouseY = pygame.mouse.get_pos()
    drawMouseHover(mouseX, mouseY, points, lastPointDone)
    renderPoints(lastPointDone, points)

    for event in pygame.event.get():
      if event.type==QUIT:
        pygame.quit()
        sys.exit()
      elif event.type == MOUSEBUTTONDOWN:
        if(mouseX <= 800 and not(lastPointDone)):
          lastPointDone = addPoint(mouseX, mouseY, points)
        elif(mouseX > 800 and lastPointDone):
          points, lastPointDone, hasPrinted = restart()

      if(lastPointDone and not(hasPrinted)):
        printPoints(points)
        hasPrinted = True
        return

    pygame.display.update()

def displayFullOutput(partition, diagonals):

  for line in partition:
    pygame.draw.line(DISPLAYSURF, BLACK, (line[0],line[1]), (line[2],line[3]), 2)
    pygame.display.update()

  d = pygame.color.THECOLORS
  for polygonDiagonals in diagonals:
    colour = choice(list(d.values()))
    colour3 = colour[:3]
    for line in polygonDiagonals:
      pygame.draw.line(DISPLAYSURF, colour3, (line[0],line[1]), (line[2],line[3]), 2)
      pygame.display.update()

def displayOutputOneLine(partition, diagonals):
  d = pygame.color.THECOLORS
  index = 0

  for line in partition:
    pygame.draw.line(DISPLAYSURF, BLACK, (line[0],line[1]), (line[2],line[3]), 2)
    pygame.display.update()
    index += 1
    yield(index)
  
  for polygonDiagonals in diagonals:
    colour = choice(list(d.values()))
    colour3 = colour[:3]
    for line in polygonDiagonals:
      pygame.draw.line(DISPLAYSURF, colour3, (line[0],line[1]), (line[2],line[3]), 2)
      pygame.display.update()
      index += 1
      yield(index)

def renderOptionsChoose():
  pygame.draw.line(DISPLAYSURF, BLACK, (815,400), (985,400), 2)
  pygame.draw.line(DISPLAYSURF, BLACK, (815,400), (815,450), 2)
  pygame.draw.line(DISPLAYSURF, BLACK, (985,450), (815,450), 2)
  pygame.draw.line(DISPLAYSURF, BLACK, (985,450), (985,400), 2)
  myfont = pygame.font.SysFont('Comic Sans MS', 20)
  mcolour = myfont.render("    Full Triangulation",True,BLACK)
  DISPLAYSURF.blit(mcolour,(800,400))


  pygame.draw.line(DISPLAYSURF, BLACK, (815,500), (985,500), 2)
  pygame.draw.line(DISPLAYSURF, BLACK, (815,500), (815,550), 2)
  pygame.draw.line(DISPLAYSURF, BLACK, (985,550), (815,550), 2)
  pygame.draw.line(DISPLAYSURF, BLACK, (985,550), (985,500), 2)
  mcolour2 = myfont.render("    Show Steps", True, BLACK)
  DISPLAYSURF.blit(mcolour2,(800,500))
  pygame.display.update()

def renderOptionSteps(partition, diagonals, counter):
  myfont = pygame.font.SysFont('Comic Sans MS', 20)
  pygame.draw.rect(DISPLAYSURF,WHITE,pygame.Rect(815,400,172,52)) 
  pygame.draw.rect(DISPLAYSURF,WHITE,pygame.Rect(815,500,170,50)) 

  pygame.draw.line(DISPLAYSURF, BLACK, (815,500), (985,500), 2)
  pygame.draw.line(DISPLAYSURF, BLACK, (815,500), (815,550), 2)
  pygame.draw.line(DISPLAYSURF, BLACK, (985,550), (815,550), 2)
  pygame.draw.line(DISPLAYSURF, BLACK, (985,550), (985,500), 2)
  mcolour2 = myfont.render("    Next Step", True, BLACK)
  DISPLAYSURF.blit(mcolour2,(800,500))
  pygame.display.update()

  generator = displayOutputOneLine(partition, diagonals)
  print("here in generator", counter)
  while(True):
    for event in pygame.event.get():
        mouseX,mouseY = pygame.mouse.get_pos()
        if event.type==QUIT:
          pygame.quit()
          sys.exit()
        elif event.type == MOUSEBUTTONDOWN:
          if(mouseX >= 802 and 500 <= mouseY <= 550):
            try:
              index = next(generator)
              if(index == counter):
                pygame.draw.rect(DISPLAYSURF,WHITE,pygame.Rect(815,500,170,50)) 
                pygame.draw.line(DISPLAYSURF, BLACK, (815,500), (985,500), 2)
                pygame.draw.line(DISPLAYSURF, BLACK, (815,500), (815,550), 2)
                pygame.draw.line(DISPLAYSURF, BLACK, (985,550), (815,550), 2)
                pygame.draw.line(DISPLAYSURF, BLACK, (985,550), (985,500), 2)
                mcolour2 = myfont.render("    Restart", True, BLACK)
                DISPLAYSURF.blit(mcolour2,(800,500))
                pygame.display.update()
            except: 
              return


def processOutput():
  file = open("formattedOutput.txt","r")
  l = file.readlines()
  file.close()

  montoneDiagonals, monotonePartition = [],[]
  index = 0
  numSides = int(l[0])
  index+=1
  counter = 0

  for i in range(numSides):
    # allLines.append([round(float(x)) for x in l[index].split()])
    index+= 1

  numMonotoneSides = int(l[index])
  index += 1

  for i in range(numMonotoneSides):
    counter += 1
    monotonePartition.append([round(float(x)) for x in l[index].split()])
    index+= 1

  numTriangles = int(l[index])
  index += 1

  for i in range(numTriangles):
    numTriangleSides = int(l[index])
    index += 1
    polygonDiagonals = []
    for j in range(numTriangleSides):
      counter += 1
      polygonDiagonals.append([round(float(x)) for x in l[index].split()])
      index+= 1
    montoneDiagonals.append(polygonDiagonals)
  
  return monotonePartition, montoneDiagonals, counter

def chooseOption():
  renderOptionsChoose()
  partition, diagonals, counter = processOutput()
  processPrinted = False
  partial = False
  while(True):
    mouseX,mouseY = pygame.mouse.get_pos()
    for event in pygame.event.get():
      if event.type==QUIT:
        pygame.quit()
        sys.exit()
      elif event.type == MOUSEBUTTONDOWN:
        if(processPrinted and mouseX >= 802):
          return
        elif(mouseX >= 802 and 400<=mouseY<=450):
          displayFullOutput(partition, diagonals)
          processPrinted = True
          # return
        elif(mouseX >= 802 and 500 <= mouseY <= 550):
          renderOptionSteps(partition, diagonals,counter)
          return

inputDone = False
while(True):
  inputPoints()
  os.system(".././a.out < formatInput.txt > formattedOutput.txt")
  chooseOption()