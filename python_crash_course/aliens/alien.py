import pygame
from pygame.sprite import Sprite

class Alien(Sprite):
    """A class for managing those pesky space aliens"""

    def __init__(self, ai_game):
        super().__init__()
        self.screen = ai_game.screen

        self.image = pygame.transform.scale(pygame.image.load('images/hank.bmp'), (100, 100))
        self.rect = self.image.get_rect()

        self.rect.x = self.rect.width
        self.rect.y = self.rect.height

        self.x = float(self.rect.x)
