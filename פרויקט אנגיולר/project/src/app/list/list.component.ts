import { Component } from '@angular/core';
import { KeyboardComponent } from '../keyboard/keyboard.component';
import { NgFor, NgIf } from '@angular/common';
import { EvasionService } from '../../services/evasion.service';

@Component({
  selector: 'app-list',
  imports: [KeyboardComponent,NgFor, NgIf],
  templateUrl: './list.component.html',
  styleUrl: './list.component.css'
})
export class ListComponent {
destinationApproved: boolean = false;
list:string[] =["1","2","3","4","5","6","7","8","9","0","q","w","e","r","t","y","u","i","o","p","a","s","d","f","g","h","j","k","l","z","x","c","v","b","n","m","delete","delete all"];
destination: string ="הכנס יעד נסיעה";
space:string="   "

  end=false;
  isReady= false;
  start=true;
constructor(private evasionService: EvasionService){}

updateDestination(letter: string){
  if(this.destination=="הכנס יעד נסיעה")
  this.destination=""
  else if(letter=="delete all"){
    this.destination=="הכנס יעד נסיעה";
      this.destination="";

  }
  
  else if(letter=="delete"){
    this.destination = this.destination .slice(0, -1)
  }

  else{
  this.destination = this.destination + letter;
  }
  
}
senddes() {
  this.isReady = true;
  this.evasionService.sendDestination(this.destination).subscribe({
    next: (approved: boolean) => {
      if (approved) {
        alert("אפשר להתחיל בנסיעה");
        this.destinationApproved = true;
      } else {
        alert("היעד לא תקין");
        this.destinationApproved = false;
      }
      this.destination = "הכנס יעד נסיעה";
      this.isReady = false;
    },
    error: (err) => {
      alert("לא ניתן להתחבר לשרת. נסה שוב מאוחר יותר.");
      this.isReady = false;
    }
  });
}
  startTraveled() {
    this.evasionService.startTravel();
    this.destinationApproved = false; // הסתר את הכפתור אחרי התחלת נסיעה
  }
}