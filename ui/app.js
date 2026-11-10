(() => {
 const $=s=>document.querySelector(s), $$=s=>Array.from(document.querySelectorAll(s));
 const drawer=$("#drawer"),quick=$("#quick-panel"),dialog=$("#app-dialog"),toast=$("#toast");let toastTimer;
 const apps=[
  {name:"NUN Core",keywords:"system core dashboard",category:"system",desc:"A future system dashboard for the NUN environment. Kernel services are not connected in this visual preview."},
  {name:"Files",keywords:"files documents storage",category:"system",desc:"The file browser will organise project folders, volumes and documents."},
  {name:"Terminal",keywords:"terminal command shell",category:"system",desc:"A future terminal surface. This visual prototype does not execute commands."},
  {name:"Studio",keywords:"studio design create",category:"creative",desc:"A creative workspace entry point for future NUN applications."},
  {name:"Gallery",keywords:"photos images gallery",category:"creative",desc:"A visual library concept for images and media."},
  {name:"Notes",keywords:"notes writing journal",category:"daily",desc:"A lightweight writing and journal workspace concept."},
  {name:"Settings",keywords:"settings controls preferences",category:"system",desc:"Display, input, accessibility and session preferences will live here."},
  {name:"Library",keywords:"library apps catalog",category:"system",desc:"An application catalog concept. No package manager is connected."},
  {name:"Phone",keywords:"calls communication",category:"daily",desc:"A phone shortcut included in the launcher concept."},
  {name:"Messages",keywords:"chat messages communication",category:"daily",desc:"A messaging shortcut included in the launcher concept."},
  {name:"Browser",keywords:"web browser internet",category:"daily",desc:"A browser shortcut included in the launcher concept."},
  {name:"Camera",keywords:"camera photo capture",category:"creative",desc:"A camera shortcut included in the launcher concept."},
  {name:"Profile",keywords:"user profile account",category:"daily",desc:"A local user profile placeholder."}
 ];
 function notify(message){toast.textContent=message;toast.classList.add("show");clearTimeout(toastTimer);toastTimer=setTimeout(()=>toast.classList.remove("show"),2100)}
 function updateClock(){const now=new Date(),time=new Intl.DateTimeFormat("en-GB",{hour:"2-digit",minute:"2-digit"}).format(now);$("#status-time").textContent=time;$("#widget-clock").innerHTML=time.replace(":",'<span>:</span>');$("#panel-clock").textContent=time;$("#widget-date").textContent=new Intl.DateTimeFormat("en-US",{month:"long",day:"2-digit",year:"numeric"}).format(now).toUpperCase().replace(","," /");$("#panel-date").textContent=new Intl.DateTimeFormat("en-US",{weekday:"short",month:"short",day:"2-digit"}).format(now).toUpperCase().replace(","," ·");const hour=now.getHours();$("#greeting").innerHTML=hour<12?'A new day, <em>a new orbit.</em>':hour<18?'Make room for <em>something new.</em>':'A softer pace, <em>your own orbit.</em>'}
 function closeOverlays(){drawer.classList.remove("open");quick.classList.remove("open");drawer.setAttribute("aria-hidden","true");quick.setAttribute("aria-hidden","true")}
 function openOverlay(panel){closeOverlays();panel.classList.add("open");panel.setAttribute("aria-hidden","false")}
 function toggleOverlay(panel){const open=panel.classList.contains("open");closeOverlays();if(!open){panel.classList.add("open");panel.setAttribute("aria-hidden","false")}}
 function launch(name){const item=apps.find(app=>app.name===name);$("#dialog-title").textContent=name;$("#dialog-copy").textContent=item?.desc||"This shortcut is part of the NUNLauncher concept.";if(dialog.showModal)dialog.showModal();else notify(name+" · preview")}
 function renderDrawer(query="",category="all"){const grid=$("#drawer-grid");grid.innerHTML="";let count=0;apps.filter(app=>(category==="all"||app.category===category)&&(!query||(app.name+" "+app.keywords).toLowerCase().includes(query))).forEach(app=>{const source=$('#home-grid [data-launch="'+app.name+'"]');if(source){const item=source.cloneNode(true);item.addEventListener("click",()=>launch(app.name));grid.append(item)}else{const item=document.createElement("button");item.className="app-item";item.dataset.launch=app.name;item.innerHTML='<span class="app-icon icon-library">✦</span><small></small>';item.querySelector("small").textContent=app.name;item.addEventListener("click",()=>launch(app.name));grid.append(item)}count++});if(!count)grid.innerHTML='<p class="empty-state">No apps found in this collection.</p>'}
 $("#all-apps").addEventListener("click",()=>{renderDrawer();toggleOverlay(drawer)});$("#dock-library").addEventListener("click",()=>{renderDrawer();toggleOverlay(drawer)});$("#drawer-close").addEventListener("click",closeOverlays);$("#quick-open").addEventListener("click",()=>toggleOverlay(quick));$("#quick-close").addEventListener("click",closeOverlays);
 $("#home-search").addEventListener("focus",()=>{if(!drawer.classList.contains("open")){renderDrawer();openOverlay(drawer)}});$("#home-search").addEventListener("input",e=>{if(!drawer.classList.contains("open"))openOverlay(drawer);$("#drawer-search").value=e.target.value;renderDrawer(e.target.value.trim().toLowerCase(),document.querySelector(".category.active")?.dataset.category||"all")});
 $("#drawer-search").addEventListener("input",e=>renderDrawer(e.target.value.trim().toLowerCase(),document.querySelector(".category.active")?.dataset.category||"all"));
 $$(".category").forEach(btn=>btn.addEventListener("click",()=>{$$(".category").forEach(item=>item.classList.toggle("active",item===btn));renderDrawer($("#drawer-search").value.trim().toLowerCase(),btn.dataset.category)}));
 $("#settings-shortcut").addEventListener("click",()=>{closeOverlays();launch("Settings")});
 $$("[data-toggle]").forEach(btn=>btn.addEventListener("click",()=>{const on=btn.getAttribute("aria-pressed")!=="true";btn.setAttribute("aria-pressed",String(on));btn.classList.toggle("active",on);btn.querySelector("small").textContent=on?(btn.dataset.toggle==="wifi"?"Connected":"On"):"Off";notify(btn.dataset.toggle+" "+(on?"enabled":"disabled"))}));
 $("#brightness").addEventListener("input",e=>{$("#phone").style.filter="brightness("+Math.max(.7,Number(e.target.value)/100)+")"});
 $$("#home-grid [data-launch]").forEach(btn=>btn.addEventListener("click",()=>launch(btn.dataset.launch)));
 $("#dialog-close").addEventListener("click",()=>dialog.close());$("#dialog-ok").addEventListener("click",()=>{dialog.close();notify("Back to your space")});dialog.addEventListener("click",e=>{if(e.target===dialog)dialog.close()});
 document.addEventListener("keydown",e=>{if(e.key==="Escape")closeOverlays()});
 updateClock();setInterval(updateClock,15000);let startY=0;
 $("#phone").addEventListener("touchstart",e=>{startY=e.changedTouches[0].clientY},{passive:true});
 $("#phone").addEventListener("touchend",e=>{const y=e.changedTouches[0].clientY,delta=y-startY;if(startY<50&&delta>60)openOverlay(quick);else if(delta< -65&&(drawer.classList.contains("open")||quick.classList.contains("open")))closeOverlays()},{passive:true});
})();