document.addEventListener('click', function (e) {
    const target = e.target.closest('.ripple-effect') || e.target.closest('[contenteditable="true"]');
    if (!target) return;

    const circle = document.createElement('span');
    const diameter = Math.max(target.clientWidth, target.clientHeight);
    const radius = diameter / 2;

    circle.style.width = circle.style.height = `${diameter}px`;
    circle.style.left = `${e.clientX - target.getBoundingClientRect().left - radius}px`;
    circle.style.top = `${e.clientY - target.getBoundingClientRect().top - radius}px`;
    circle.classList.add('ripple');

    const oldRipple = target.querySelector('.ripple');
    if (oldRipple) oldRipple.remove();

    target.appendChild(circle);
});

const editableElements = document.querySelectorAll('[contenteditable="true"]');

editableElements.forEach((el, index) => {
    const savedValue = localStorage.getItem(`resume-item-${index}`);
    if (savedValue) el.innerText = savedValue;

    el.addEventListener('input', () => {
        localStorage.setItem(`resume-item-${index}`, el.innerText);
    });
});

document.getElementById('download-pdf').addEventListener('click', () => {
    window.print();
});
