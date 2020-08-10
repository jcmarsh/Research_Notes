import pygame

class Ship:
    """Class to manage the ship"""

    def __init__(self, ai_game):
        """Init the ship and set it's position"""
        self.screen = ai_game.screen
        self.screen_rect = ai_game.screen.get_rect()
        self.settings = ai_game.settings

        # Load the ship image and get its rect
        self.image = pygame.transform.scale(pygame.image.load('images/astronaut_cat.bmp'), (100, 100))
        self.rect = self.image.get_rect()

        # Start the ship in the bottom center
        self.rect.midbottom = self.screen_rect.midbottom

        self.x = float(self.rect.x)

        # Set movement flags
        self.moving_right = False
        self.moving_left = False

    def update(self):
        if self.moving_right and self.rect.right < self.screen_rect.right:
            self.x += self.settings.ship_speed
        if self.moving_left and self.rect.left > self.screen_rect.left:
            self.x -= self.settings.ship_speed

        self.rect.x = self.x

    def blitme(self):
        """Draw the ship"""
        self.screen.blit(self.image, self.rect)
