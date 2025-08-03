import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Observable, map } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class EvasionService {
  addrrserver = "http://localhost:9090/evasion";
  addresserver2 = "http://localhost:9090/travel";

  constructor(private httpClient: HttpClient) { }

  sendDestination(destination: string): Observable<boolean> {
    return this.httpClient.post(
      this.addrrserver,
      destination,
      { responseType: 'text' }
    ).pipe(
      map(res => {
        console.log('server response:', JSON.stringify(res));
        // בדיקה גמישה יותר, תומכת גם ב-"true" עם תווים נוספים
        return res.trim().toLowerCase().startsWith('true');
      })
    );
  }

  startTravel(): void {
    this.httpClient.post(
      this.addresserver2,
      {},
      { responseType: 'text' }
    ).subscribe();
  }
}