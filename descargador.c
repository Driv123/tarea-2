#include <libgen.h>  
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <curl/curl.h>
#include <string.h>


int progress_callback(void* p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
    if (dltotal > 0) {
        double percentage = (double)dlnow / (double)dltotal * 100.0;
        printf("\rDescargando: %.2f%% completado", percentage);
        fflush(stdout); 
    }
    return 0;  
}

void *download_file(void *url) {
    CURL *curl;
    FILE *fp;
    CURLcode res;
    char *file_url = (char *)url;
    char outfilename[FILENAME_MAX];

    strcpy(outfilename, basename(file_url));

    curl = curl_easy_init();
    if (curl) {
        fp = fopen(outfilename, "wb");
        if (!fp) {
            perror("Error al abrir archivo");
            return NULL;
        }

        curl_easy_setopt(curl, CURLOPT_URL, file_url);

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);


        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progress_callback);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L); 

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "Error en la descarga de %s: %s\n", file_url, curl_easy_strerror(res));
        } else {
            printf("\nDescarga completada: %s\n", outfilename);
        }

   
        fclose(fp);

        curl_easy_cleanup(curl);
    }

    return NULL;
}

int main() {
    const char *urls[] = {
        "https://www.escom.ipn.mx/docs/oferta/uaoISC2020/machineLearning.pdf",
        "https://www.escom.ipn.mx/docs/oferta/uaoISC2020/geneticAlgorithms.pdf",
        "https://www.escom.ipn.mx/docs/oferta/uaoISC2020/computerGraphics.pdf"
    };

    int num_urls = sizeof(urls) / sizeof(urls[0]);


    pthread_t threads[num_urls];


    curl_global_init(CURL_GLOBAL_DEFAULT);

    for (int i = 0; i < num_urls; i++) {
        if (pthread_create(&threads[i], NULL, download_file, (void *)urls[i])) {
            fprintf(stderr, "Error creando el hilo para la URL: %s\n", urls[i]);
            return 1;
        }
    }

  
    for (int i = 0; i < num_urls; i++) {
        pthread_join(threads[i], NULL);
    }

    curl_global_cleanup();

    printf("Descargas completadas.\n");

    return 0;
}
