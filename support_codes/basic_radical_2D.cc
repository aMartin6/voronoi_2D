// Radical Voronoi tessellation example code
//
// Original Author   : Chris H. Rycroft (LBL / UC Berkeley)
// Email    : chr@alum.mit.edu
// Date     : August 30th 2011
// Modifications to 2D, PBC: Danielle McDermott
// Email    : mcd.danielle@gmail.com
// Date     : June 19th, 2016

#include <vector>
using namespace std;

#include "voro++.hh"
using namespace voro;

FILE *fp;

// Set up constants for the container geometry
const double x_min=0,x_max=60;
const double y_min=0,y_max=60;
const double z_min=-0.5,z_max=0.5;

// Set up the number of blocks that the container is divided
// into.
const int n_x=20,n_y=20,n_z=1;

int main() {

  	double x,y,z;
  	voronoicell_neighbor c;
	vector<int> neigh;
	int i;
	
	// Create a container for polydisperse particles using the same
	// geometry as above. Import the polydisperse test packing and
	// output the Voronoi radical tessellation in gnuplot and POV-Ray
	// formats.
        container_poly conp(x_min,x_max,            //container size
			    y_min,y_max,  
			    z_min,z_max,  
			    n_x,n_y,n_z,            //number threads
			    true,true,false,        //pbc in x/y
			    16);                     //number particles/block
	conp.import("poly_output_voro");             //get the data 
	conp.draw_cells_gnuplot("poly.gnu");         //do the tessellation
	conp.draw_cells_pov("poly_v.pov");
	conp.draw_particles_pov("poly_p.pov");

	//---------------------------------------------------
	//analyze the data for number of voronoi faces
	//---------------------------------------------------
	//open a file
	if((fp = fopen("voro_stats.txt","w"))==NULL)
	  {printf("Cannot open file.\n");
	    exit(1);
	  }

	//array for storing voronoi stats
        int pN[4];

	//initialize values
	pN[0]=0;
	pN[1]=0;
	pN[2]=0;
	pN[3]=0;

	int n_neigh;
	int normalize = 0; // = conp.number_of_partcilcsl;
	  
	//from http://math.lbl.gov/voro++/examples/polygons/
	// Loop over all particles in the container
	// and compute each Voronoi cell
	//do I need c_loop_all_periodic? radical?

	//printf("%d\n",conp.id[0]);
	
	c_loop_all cl(conp);
	if(cl.start()) do if(conp.compute_cell(c,cl)) {
	      cl.pos(x,y,z);
	      //id=cl.pid();

	      normalize += 1;
	      
	      // Gather information about the computed Voronoi cell
	      c.neighbors(neigh);
	      n_neigh = neigh.size()-2;  //subtract off the z-faces
	      
	      if(n_neigh >3 && n_neigh<8){
		pN[n_neigh-4] += 1;
	      }

	    } while (cl.inc());


	printf("%d\n",normalize);
	for(i=0;i<4;i++){
	  printf("p%d, %d, %f\n",i, pN[i], (float)pN[i]/ (float)normalize);
	}

	fclose(fp);
	
}
