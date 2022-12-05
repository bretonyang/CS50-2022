class Website {
  image = document.querySelector('.hero-img');
  count;

  constructor() {
    this.count = 0;
    this.addListenerImage();
  }

  addListenerImage() {
    this.image.addEventListener('click', (e) => {
      this.count++;
      if (this.count == 10) {
        alert('Keep clicking for an easter egg!');
      } else if (this.count == 20) {
        alert('Your life must be boring to be clicking an image for 20 times');
      }
    })
  }
}

const init = new Website();
