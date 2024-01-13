#include "Vec3D.h"
#include "Rayon.h"
#include "Scene.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <limits>
#include <random>
#include "Pool.h"
#include "Barrier.h"

using namespace std;
using namespace pr;


void fillScene(Scene & scene, default_random_engine & re) {
	// Nombre de spheres (rend le probleme plus dur)
	const int NBSPHERES = 250;

	// on remplit la scene de spheres colorees de taille position et couleur aleatoire
	uniform_int_distribution<int> distrib(0, 200);
	uniform_real_distribution<double> distribd(-200, 200);
	for (int i = 0; i < NBSPHERES; i++) {
		// position autour de l'angle de la camera
		// rayon entre 3 et 33, couleur aleatoire
		// distrib(re) rend un entier aleatoire extrait de re
		scene.add(Sphere({50+distribd(re),50 + distribd(re),120 + distribd(re) }, double(distrib(re)%30) + 3.0, Color::random()));
	}
	// quelques spheres de plus pour ajouter du gout a la scene
	scene.add(Sphere({50,50,40},15.0,Color::red));
	scene.add(Sphere({100,20,50},55.0,Color::blue));

}

// return the index of the closest object in the scene that intersects "ray"
// or -1 if the ray does not intersect any object.
int findClosestInter(const Scene & scene, const Rayon & ray) {
	auto minz = std::numeric_limits<float>::max();
	int targetSphere = -1;
	int index = 0;
	for (const auto & obj : scene) {
		// rend la distance de l'objet a la camera
		auto zinter = obj.intersects(ray);
		// si intersection plus proche  ?
		if (zinter < minz) {
			minz = zinter;
			targetSphere = index;
		}
		index++;
	}
	return targetSphere;
}

// Calcule l'angle d'incidence du rayon à la sphere, cumule l'éclairage des lumières
// En déduit la couleur d'un pixel de l'écran.
Color computeColor(const Sphere & obj, const Rayon & ray, const Vec3D & camera, std::vector<Vec3D> & lights) {
	Color finalcolor = obj.getColor();

	// calcul du rayon et de sa normale a la sphere
	// on prend le vecteur de la camera vers le point de l'ecran (dest - origine)
	// on le normalise a la longueur 1, on multiplie par la distance à l'intersection
	Vec3D rayInter = (ray.dest - ray.ori).normalize() * obj.intersects(ray);
	// le point d'intersection
	Vec3D intersection = rayInter + camera;
	// la normale a la sphere au point d'intersection donne l'angle pour la lumiere
	Vec3D normal = obj.getNormale(intersection);
	// le niveau d'eclairage total contribue par les lumieres 0 sombre 1 total lumiere
	double dt = 0;
	// modifier par l'eclairage la couleur
	for (const auto & light : lights) {
		// le vecteur de la lumiere au point d'intersection
		Vec3D tolight = (light - intersection);
		// si on est du bon cote de la sphere, i.e. le rayon n'intersecte pas avant de l'autre cote
		if (obj.intersects(Rayon(light,intersection)) >= tolight.length() - 0.05 ) {   //  epsilon 0.05 for double issues
			dt += tolight.normalize() & normal ; // l'angle (scalaire) donne la puissance de la lumiere reflechie
		}
	}
	// eclairage total
	finalcolor = finalcolor * dt + finalcolor * 0.2; // *0.2 = lumiere speculaire ambiante

	return finalcolor;
}

// produit une image dans path, représentant les pixels.
void exportImage(const char * path, size_t width, size_t height, Color * pixels) {
	// ppm est un format ultra basique
	ofstream img (path);
	// P3 signifie : les pixels un par un en ascii
	img << "P3" << endl; // ascii format, colors
	// largeur hauteur valeur max d'une couleur (=255 un char)
	img << width  << "\n"<< height << "\n" << "255" << endl;
	// tous les pixels au format RGB
	for (size_t  y = 0 ; y < height ; y++) {
		for (size_t x =0 ; x < width ; x++) {
			Color & pixel = pixels[x*height + y];
			img << pixel << '\n';
		}
	}
	// oui ca fait un gros fichier :D
	img.close();
}

class RayJob : public Job {
	Scene & m_scene;
	const Scene::screen_t & m_screen;
	Color * m_pixels;
	int m_x;
	int m_y;
	vector<Vec3D> & m_lights;
	Barrier & m_barrier;
	public:
		RayJob(Scene & scene, const Scene::screen_t & screen, Color * pixels, int x, int y, vector<Vec3D> & lights, Barrier & barrier) :
			m_scene(scene),
			m_screen(screen),
			m_pixels(pixels),
			m_x(x),
			m_y(y),
			m_lights(lights),
			m_barrier(barrier)
	{}
		void run() {
			if(m_x == -1) {
				int width = m_scene.getWidth();
				for(int x = 0; x < width; ++x) {
					m_x = x;
					compute_color();
				}
			}
			else {
				compute_color();
			}
			m_barrier.done();
		}
		void compute_color() {
			// le point de l'ecran par lequel passe ce rayon
			auto & screenPoint = m_screen[m_y][m_x];
			// le rayon a inspecter
			Rayon ray(m_scene.getCameraPos(), screenPoint);

			int targetSphere = findClosestInter(m_scene, ray);

			if (targetSphere == -1) {
				// keep background color
				return;
			} else {
				const Sphere & obj = *(m_scene.begin() + targetSphere);
				// pixel prend la couleur de l'objet
				Color finalcolor = computeColor(obj, ray, m_scene.getCameraPos(), m_lights);
				// le point de l'image (pixel) dont on vient de calculer la couleur
				Color & pixel = m_pixels[m_y*m_scene.getHeight() + m_x];
				// mettre a jour la couleur du pixel dans l'image finale.
				pixel = finalcolor;
			}
		}
};

enum class JOB_GRAIN
{
	PIXEL,
	LINE
};

// NB : en francais pour le cours, preferez coder en english toujours.
// pas d'accents pour eviter les soucis d'encodage

int main () {

	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	// on pose une graine basee sur la date
	default_random_engine re(std::chrono::system_clock::now().time_since_epoch().count());
	// definir la Scene : resolution de l'image
	Scene scene (1000,1000);
	// remplir avec un peu d'aléatoire
	fillScene(scene, re);
	
	// lumieres 
	vector<Vec3D> lights;
	lights.reserve(3);
	lights.emplace_back(Vec3D(50, 50, -50));
	lights.emplace_back(Vec3D(50, 50, 120));
	lights.emplace_back(Vec3D(200, 0, 120));

	// les points de l'ecran, en coordonnées 3D, au sein de la Scene.
	// on tire un rayon de l'observateur vers chacun de ces points
	const Scene::screen_t & screen = scene.getScreenPoints();

	// Les couleurs des pixels dans l'image finale
	Color * pixels = new Color[scene.getWidth() * scene.getHeight()];

	// get number of threads
	unsigned int numThreads = std::thread::hardware_concurrency();
	cout << "Raytracing with " << numThreads << " threads" << endl;
	
	// grain du parallelisme
	JOB_GRAIN grain = JOB_GRAIN::LINE;

	if(grain == JOB_GRAIN::PIXEL)
	{
		// create pool of threads and sync barrier
		unsigned int numJobs = scene.getWidth() * scene.getHeight();
		Pool threadPool(numJobs);
		threadPool.start(numThreads);
		Barrier barrier(numJobs);

		// pour chaque pixel, calculer sa couleur 
		for (int x =0 ; x < scene.getWidth() ; x++) {
			for (int  y = 0 ; y < scene.getHeight() ; y++) {
				threadPool.submit(new RayJob(scene, screen, pixels, x, y, lights, barrier));
			}
		}
		barrier.waitFor();
		threadPool.stop();
	}
	else if(grain == JOB_GRAIN::LINE)
	{
		// create pool of threads and sync barrier
		unsigned int numJobs = scene.getHeight();
		Pool threadPool(numJobs);
		threadPool.start(numThreads);
		Barrier barrier(numJobs);

		// pour chaque pixel, calculer sa couleur 
		for (int  y = 0 ; y < scene.getHeight() ; y++) {
			threadPool.submit(new RayJob(scene, screen, pixels, -1, y, lights, barrier));
		}
		barrier.waitFor();
		threadPool.stop();
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	    std::cout << "Total time "
	              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
	              << "ms.\n";

	exportImage("toto.ppm",scene.getWidth(), scene.getHeight() , pixels);

	return 0;
}

