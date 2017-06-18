"""
Treemap builder using pylab.

Uses algorithm straight from http://hcil.cs.umd.edu/trs/91-03/91-03.html

James Casbon 29/7/2006
"""

import pylab
from matplotlib.patches import Rectangle

class Proctmap:
    def __init__(self, t_base, t_proct, size_method):
        width = 1 # Not needed: thought could adjust this way but changing aspect is easier.
        
        self.ax = pylab.subplot(111, aspect='equal')
        pylab.subplots_adjust(left=0, right=width, top=1, bottom=0)
        self.ax.set_xticks([])
        self.ax.set_yticks([])

        axis = 0
        lower = [0,0]
        upper = [width,1]
        self.draw_rectangle(lower, upper, 'y')

        count = 0
        #for area in t_base:
        for comp in t_proct:
            # Assumes that t_proc has n top elements, where n is the number of elements in t_base
            p_width = float(size(comp)) / size(t_proct)
            p_height = float(comp[0]) / size(comp)
            # rect for t_proc
            upper = [lower[0] + width * p_width, 1 * p_height]
            self.draw_rectangle(lower, upper, '#fc8d62')
            next_low = [upper[0], lower[1]]
            
            # rect for t_base
            # This is the baseline the voter has to beat.
            line_y = 1 * float(t_base[count]) / size(comp)
            #self.draw_rectangle(lower, [upper[0], line_y], 'r')
            #self.ax.plot([lower[0], upper[0]], [line_y, line_y], color='r', linestyle='-', linewidth=2)

            # Apparantly hatch marks can't be handled by matplotlib: https://github.com/matplotlib/matplotlib/issues/3841
            #r = Rectangle(lower, upper[0]-lower[0], line_y - lower[1], edgecolor='k', facecolor='w', hatch='X', alpha=0.5)
            r = Rectangle(lower, upper[0]-lower[0], line_y - lower[1], edgecolor='k', facecolor='w', alpha=0.5)

            count = count + 1
            
            # t_base children
            lower[1] = upper[1]
            p_width = p_width / len(comp[1])
            #p_height = float(comp[0]) / size(comp)
            for rep in comp[1]:
                # rect for rep
                upper = [lower[0] + width * p_width, 1]
                self.draw_rectangle(lower, upper, '#8da0cb')
                lower[0] = upper[0]
            lower = next_low
            self.ax.add_patch(r)
            
    def draw_rectangle(self, lower, upper, color):
        print lower, upper
        r = Rectangle(lower, upper[0]-lower[0], upper[1] - lower[1],
                      edgecolor='k',
                      facecolor=color)
        self.ax.add_patch(r)

                                         
if __name__ == '__main__':
    # example using nested lists, iter to walk and random colors

    size_cache = {}
    def size(thing):
        if isinstance(thing, int):
            return thing
        if thing in size_cache:
            return size_cache[thing]
        else:
            size_cache[thing] = reduce(int.__add__, [size(x) for x in thing])
            return size_cache[thing]
    import random
    def random_color(thing):
        return (random.random(),random.random(),random.random())

    # L4 TMR 0% benign
    #tree_protected = ((69, (45, 45, 45)), (59, (20, 20, 20)), (163, (2087, 2087, 2087)), (106, (24, 24, 24)))
    # tree = ((69, (45, 45, 45)), (59, (20, 20, 20)), (106, (24, 24, 24)), (82, (1044, 1044, 1044)))
    # L4 NMR 0% benign
    #tree_baseline = ((36, 35, 1299, 48))

    # BBB DMR
    tree_protected = ((243, (122, 122)), (162, (146, 146)), (398, (5117, 5117)), (277, (79, 79)))
    # BBB NMR
    tree_baseline = (136, 152, 3851, 63)

    # Example testing
    #tree_protected = ((2, (1, 1)), (1, (1, 1, 1)), (4, (2, 2)))
    #tree_baseline = (1, 2, 3)
    
    # tree = ((135.9555516378, 151.6977794622, 3851.11998298, 62.9688898311)) # floats don't work
    #tree = ((136, 152, 3851, 63))
    # tree= ((5,(3,5)), 4, (5,2,(2,3,(3,2,2)),(3,3)), (3,2) )
    # Treemap(tree, iter, size, random_color)
    Proctmap(tree_baseline, tree_protected, size)

    pylab.savefig('out.pdf', bbox_inches='tight', pad_inches=0)
    pylab.show()
