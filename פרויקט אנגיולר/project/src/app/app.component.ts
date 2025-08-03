import { Component } from '@angular/core';
import { RouterOutlet } from '@angular/router';
import { DestinationComponent } from './destination/destination.component';
import { ListComponent } from './list/list.component';
import { HeaderComponent } from './header/header.component';

@Component({
  selector: 'app-root',
  imports: [RouterOutlet, DestinationComponent, ListComponent,HeaderComponent],
  templateUrl: './app.component.html',
  styleUrl: './app.component.css'
})
export class AppComponent {
  title = 'project';
}
