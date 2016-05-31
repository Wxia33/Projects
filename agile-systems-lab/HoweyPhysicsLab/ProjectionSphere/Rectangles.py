import pygame

pygame.init()

white = (255,255,255)
black = (0,0,0)
red = (255,0,0)
blue =  (0,0,255)
green = (0,255,0)
pi = 3.141592

#display = pygame.display.set_mode((1400,800))
display = pygame.display.set_mode((1400, 700),0,0)
pygame.display.set_caption('Slither')

gameExit = False

lead_x = 750
lead_y = 0
lead_x_change = 0
curve = 0
clock = pygame.time.Clock()

while not gameExit:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            gameExit = True
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_LEFT:
                lead_x -= 50
                curve -= 50
            if event.key == pygame.K_RIGHT:
                lead_x += 50
                curve += 50
            if event.key == pygame.K_UP:
                lead_y -= 50
            if event.key == pygame.K_DOWN:
                lead_y += 50
                
        lead_x += lead_x_change
        display.fill(white)
        if lead_x > 550 and lead_x < 1000:
            pygame.draw.rect(display, black, [lead_x,lead_y,50,700])
        elif lead_x >= 1000:
#            pygame.draw.arc(display, black,[lead_x,lead_y,200,500], -pi/5, pi/5, 50)
            pygame.draw.arc(display, black,[lead_x - 150,lead_y - (530 - curve / 2),200,1600 - curve], -pi/((-curve/300)+5), pi/((-curve/300)+5), 50)
        else:
            pygame.draw.arc(display, black,[lead_x,lead_y - (530 - curve / 2),200,1600 - curve], -pi/((-curve/300)+5) + pi, pi/((-curve/300)+5) + pi, 50) 
#            pygame.draw.arc(display, black,[lead_x - 200,lead_y - 580,200,1700], -pi/19, pi/13, 50)
        pygame.display.update()
        clock.tick(10)

pygame.quit()

quit()
