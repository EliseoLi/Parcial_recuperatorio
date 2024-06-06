#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include<math.h>

struct Header1
{
    uint16_t rsv:3;
    uint16_t upper_level_device_id:10;
    uint16_t rsv:3;
};
struct Header2
{   
    uint16_t device_type_NL:4;
    uint16_t info:8;
    uint16_t device_type_NH:4;
};
struct Header3
{
    uint16_t lower_level_devices_count:6;
    uint16_t ID:10;
};


/**
 * @brief Funcion extract16
 * 
 * Esta funcion sirve para extraer los bits de tipo 16.
 * @param num_bits  Número de bits a extraer
 * @param mask  Máscara para extraer los bits
 * @returns (data >> inicio) & mask;  Desplaza a la derecha y aplica máscara
 * 
 */
//Funcion para extraer bit de 16 bits 
uint16_t extract16(uint16_t data, uint32_t inicio, uint32_t final) {
    uint32_t num_bits = final - inicio + 1;
    uint16_t mask = (1U << num_bits) - 1;
    return (data >> inicio) & mask;
}

/**
 * @brief Funcion extractHeader
 * 
 * Funcion para extraer los datos del mapa de bits haciendo uso tambien de extract16.
 * @param head  Renombramos a nuestra estructura en la funcion.
 * @param encabezadoData  data en el cual se usa la funcion extract16 para ir al bit a usar.
 * @returns head (Para usar obviamente en el main)
 * 
 */
//Funcion para extraer cada bits del header


Header1 extractHeader1(uint16_t encabezadoData) {
    Header1 head;
    head.upper_level_device_id = extract16(encabezadoData, 3, 12);
    return head;
}
/**
 * @brief Funcion extractHeader2
 * 
 * Funcion para extraer los datos del mapa de bits haciendo uso tambien de extract16.
 * @param head  Renombramos a nuestra estructura en la funcion.
 * @param encabezadoData  data en el cual se usa la funcion extract16 para ir al bit a usar.
 * @returns head (Para usar obviamente en el main)
 * 
 */
//Funcion para extraer cada bits del header
Header2 extractHeader2(uint16_t encabezadoData) 
{
    Header2 head;
    head.device_type_NL=extract16(encabezadoData,0,3);
    head.info = extract16(encabezadoData, 4, 11);
    head.device_type_NH=extract16(encabezadoData,12,15);
    return head;
}
/**
 * @brief Funcion extractHeader3
 * 
 * Funcion para extraer los datos del mapa de bits haciendo uso tambien de extract16.
 * @param head  Renombramos a nuestra estructura en la funcion.
 * @param encabezadoData  data en el cual se usa la funcion extract16 para ir al bit a usar.
 * @returns head (Para usar obviamente en el main)
 * 
 */
//Funcion para extraer cada bits del header
Header3 extractHeader3(uint16_t encabezadoData) 
{
    Header3 head;
    head.lower_level_devices_count = extract16(encabezadoData, 0, 5);
    head.ID = extract16(encabezadoData,6, 15);
    return head;
}


void showCampo2(struct Header2 head) 
{
    int count;
    for (int i=0;i<count;i++) 
    {
        printf("--------------\n");
        if(head.device_type_NL==0 && head.device_type_NH==0)
        {
            printf("CPU");
        }
        if (head.device_type_NL==1 && head.device_type_NH==0)
        {
            printf("SENSOR: ");
            if (head.info==0)
                printf("DEL TIPO FLOW");
            else if (head.info==1)
                printf("DEL TIPO TEMP");
            else if (head.info==2)
                printf("DEL TIPO PRESION");
            else if (head.info==3)
                printf("DEL TIPO NIVEL");   
        }
        if (head.device_type_NL==0 && head.device_type_NH==1)
        {
            printf("ACTUADOR: ");
            if (head.info==0)
                printf("DEL TIPO VALVULA");
            else if(head.info==1)
                printf("DEL TIPO MOTOR");    
        }
        if (head.device_type_NL==1 && head.device_type_NH==1)
        {
            printf("CONCENTRATOR: ");
        }       
    }
}

// Función para mostrar la secuencia de conexión de un dispositivo
void showConnectionSequence(struct Header1 head, uint16_t id) {
    while (id != 0xFFFF) { // 0xFFFF indica que no hay conexión superior
        printf("ID: %u\n", id);
        bool found = false;//Variable bool tipo bandera incializada en falso
        int count=32;
        for (int i=0;i<count;i++) 
        {
            if (head.upper_level_device_id == id) 
            {
                id = head.upper_level_device_id;
                found = true; // Hemos encontrado el ID, así que marcamos found como true
                break; // Salimos del bucle ya que hemos encontrado el ID
            }
        }
        if (!found) break; // Si no se encuentra el ID, se termina la secuencia
    }
}

int main()
{
    FILE *f=fopen("network_structure.dat","rb");
    int cant;
    fseek(f,0,SEEK_END);
    cant=(ftell(f)/sizeof(struct Header1)); //Cantidad de datos
    printf("\n\nCantidad de paquetes:%d\n\n",cant);
    fseek(f,0,SEEK_SET);
    //Cantidad de paquetes es 39

    //Creamos memoria dinamica para el header1
    struct Header1 *data=new struct Header1[cant];
    int result = fread(data, sizeof(Header1), cant, f);
    if (result != cant) 
    {
        printf("Error al leer los datos\n");
        delete[] data;
        fclose(f);
        return 1;
    }
 int cant2;
    fseek(f,0,SEEK_END);
    cant2=(ftell(f)/sizeof(struct Header2)); //Cantidad de datos
    printf("\n\nCantidad de paquetes:%d\n\n",cant2);
    fseek(f,0,SEEK_SET);
//Creamos memoria dinamica para el Header2
struct Header2 *data2=new struct Header2[cant2];
int result = fread(data2, sizeof(Header2), cant2, f);
    if (result != cant2) 
    {
        printf("Error al leer los datos\n");
        delete[] data;
        fclose(f);
        return 1;
    }
//LEEMOS EL 3ER HEADER
int cant3;
    fseek(f,0,SEEK_END);
    cant3=(ftell(f)/sizeof(struct Header3)); //Cantidad de datos
    printf("\n\nCantidad de paquetes:%d\n\n",cant3);
    fseek(f,0,SEEK_SET);
struct Header3 *data3=new struct Header3[cant3];
int result = fread(data3, sizeof(Header3), cant2, f);
    if (result != cant3) 
    {
        printf("Error al leer los datos\n");
        delete[] data;
        fclose(f);
        return 1;
    }
    fclose(f);
    delete[] data;
    return 0;
}