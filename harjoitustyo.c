#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#define COUNT 4

typedef struct puusolmu_t
{
  int luku;
  short tila; /* tasapainoilmaisin */
  struct puusolmu_t *vasen, *oikea;
} puusolmu, *puuosoitin;

void lisaa_solmu(puuosoitin *, int, int *);
void oikea_kierto(puuosoitin *, int *);
void tulosta_puu(puuosoitin, int space);
void vasen_kierto(puuosoitin *, int *);
void etsi_puusta(puuosoitin, int alkio);

/* Pääohjelma jossa tiedoston luku, puun luonti, valikko toiminnot */
int main()
{
  int alkio = 0; /* luku jonka avulla tarkistetaan luvun olemassa olo puussa ja mahdollisesti lisätään luku puuhun*/
  int valinta = 1;
  int etp = 0;
  puuosoitin puu = NULL;

  FILE *tiedosto;
  tiedosto = fopen("test.txt", "r");
  char rivi[12];
  printf("Lisätään puuhun solmut: ");
  while (fgets(rivi, sizeof(rivi), tiedosto))
  {
    int num = atoi(rivi);
    printf("%d, ", num);
    lisaa_solmu(&puu, num, &etp);
    /* tulosta_puu(puu, 0);
    printf("------------------------------------------------------------------------------------------------\n"); */
  }
  fclose(tiedosto);
  printf("\n\n");
  tulosta_puu(puu, 0);

  /*Valikko*/

  while (valinta != 0)
  {
    printf("1) Etsi luku puusta\n2) Lisää yksittäis avain\n0) Lopeta\n");
    printf("Valinta: ");
    scanf("%d", &valinta);
    if (valinta == 1) /*Solmun etsiminen puusta*/
    {
      printf("Anna avain jonka haluat tarkistaa: ");
      scanf("%d", &alkio);
      etsi_puusta(puu, alkio);
    }
    else if (valinta == 2) /* Solmun lisääminen puuhun */
    {
      printf("Anna avain jonka haluat lisätä puuhun: ");
      scanf("%d", &alkio);
      lisaa_solmu(&puu, alkio, &etp);
      printf("\n\n");
      tulosta_puu(puu, 0);
    }
    else if (valinta == 0)
    {
      break;
    }
    else
    {
      printf("Vain valinnat 1, 2 ja 0 käyvät!!!\n\n");
    }
  }
  printf("Ohjelma suljetaan, kiitos käytöstä.\n");
  return 0;
}

/* Puun muodostaminen */

void lisaa_solmu(puuosoitin *emo, int luku, int *etp)
{
  if (!(*emo))
  {
    *etp = 1;
    if (!(*emo = (puuosoitin)malloc(sizeof(puusolmu))))
    {
      perror("malloc");
      exit(1);
    }
    (*emo)->vasen = (*emo)->oikea = NULL;
    (*emo)->tila = 0;
    (*emo)->luku = luku;
  }
  else if (luku > (*emo)->luku)
  {
    lisaa_solmu(&(*emo)->vasen, luku, etp);
    if (*etp)
    {
      switch ((*emo)->tila)
      {
      case -1:
        (*emo)->tila = 0;
        *etp = 0;
        break;
      case 0:
        (*emo)->tila = 1;
        break;
      case 1:
        vasen_kierto(emo, etp);
      }
    }
  }
  else if (luku < (*emo)->luku)
  {
    lisaa_solmu(&(*emo)->oikea, luku, etp);
    if (*etp)
    {
      switch ((*emo)->tila)
      {
      case 1:
        (*emo)->tila = 0;
        *etp = 0;
        break;
      case 0:
        (*emo)->tila = -1;
        break;
      case -1:
        oikea_kierto(emo, etp);
      }
    }
  }
  else
  {
    *etp = 0;
    printf("Luku %d on jo puussa\n", luku);
  }
}

/* Etsi alkio puusta */

void etsi_puusta(puuosoitin solmu, int alkio)
{
  if (!solmu)
  {
    printf("\nAvain ei ole puussa!\n\n");
    return;
  }
  if (solmu->luku == alkio)
  {
    printf("\nAvain %d löytyy!\n\n", solmu->luku);
  }
  else if (solmu->luku < alkio)
  {
    etsi_puusta(solmu->vasen, alkio);
  }
  else if (solmu->luku > alkio)
  {
    etsi_puusta(solmu->oikea, alkio);
  }
  else
  {
    printf("\nAvain ei ole puussa!\n\n");
  }
}

/* Puun tulostus */

void tulosta_puu(puuosoitin solmu, int space)
{
  int i;

  if (!solmu)
    return;

  space += COUNT;

  tulosta_puu(solmu->vasen, space);

  for (i = COUNT; i < space; i++)
  {
    printf(" ");
  }
  printf("%d\n", solmu->luku);

  tulosta_puu(solmu->oikea, space);
}

/* Vasen kierto */

void vasen_kierto(puuosoitin *emo, int *etp)
{
  puuosoitin lapsi, lapsenlapsi;

  lapsi = (*emo)->vasen;
  if (lapsi->tila == 1) /* LL-kierto */
  {
    printf("LL-kierto, ");
    /* tulosta_puu(*emo, 0);
    printf("------------------------------------------------------------------------------------------------\n"); */
    (*emo)->vasen = lapsi->oikea;
    lapsi->oikea = *emo;
    (*emo)->tila = 0;
    (*emo) = lapsi;
  }
  else /* LR-kierto */
  {
    printf("LR-kierto, ");
    /* tulosta_puu(*emo, 0);
    printf("------------------------------------------------------------------------------------------------\n"); */
    lapsenlapsi = lapsi->oikea;
    lapsi->oikea = lapsenlapsi->vasen;
    lapsenlapsi->vasen = lapsi;
    (*emo)->vasen = lapsenlapsi->oikea;
    lapsenlapsi->oikea = *emo;
    switch (lapsenlapsi->tila)
    {
    case 1:
      (*emo)->tila = -1;
      lapsi->tila = 0;
      break;
    case 0:
      (*emo)->tila = lapsi->tila = 0;
      break;
    case -1:
      (*emo)->tila = 0;
      lapsi->tila = 1;
    }
    *emo = lapsenlapsi;
  }
  (*emo)->tila = 0;
  *etp = 0;
}

/* Oikea kierto */

void oikea_kierto(puuosoitin *emo, int *etp)
{
  puuosoitin lapsi, lapsenlapsi;

  lapsi = (*emo)->oikea;
  if (lapsi->tila == -1) /* RR-kierto */
  {
    printf("RR-kierto, ");
    /* tulosta_puu(*emo, 0);
    printf("\n------------------------------------------------------------------------------------------------\n"); */
    (*emo)->oikea = lapsi->vasen;
    lapsi->vasen = *emo;
    (*emo)->tila = 0;
    (*emo) = lapsi;
  }
  else /* RL-kierto */
  {
    printf("RL-kierto, ");
    /* tulosta_puu(*emo, 0);
    printf("\n------------------------------------------------------------------------------------------------\n"); */
    lapsenlapsi = lapsi->vasen;
    lapsi->vasen = lapsenlapsi->oikea;
    lapsenlapsi->oikea = lapsi;
    (*emo)->oikea = lapsenlapsi->vasen;
    lapsenlapsi->vasen = *emo;
    switch (lapsenlapsi->tila)
    {
    case 1:
      (*emo)->tila = -1;
      lapsi->tila = 0;
      break;
    case 0:
      (*emo)->tila = lapsi->tila = 0;
      break;
    case -1:
      (*emo)->tila = 0;
      lapsi->tila = 1;
    }
    *emo = lapsenlapsi;
  }
  (*emo)->tila = 0;
  *etp = 0;
}