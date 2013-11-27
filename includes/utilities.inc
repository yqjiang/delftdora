<?php

/**
 * @file
 * Miscellaneous helper functions.
 */

/**
 * Get The location of buidings (Object with model:Building model).
 * 
 * @param int $offset
 *   The offset of buildings.
 * @param int $limit
 *   The number of buildings that will be return.
 * 
 * @return array 
 *   Locations of buildings
 */
function delftdora_get_building_locations($offset = 0, $limit = -1) {
  $query = "SELECT ?pid ?object ?position ?title FROM <#ri> WHERE 
            {?object <fedora-model:hasModel> <info:fedora/model:Building> ;
                        <fedora-model:label> ?title ;
                        <dc:identifier> ?pid;
                        <dc:coverage> ?position;
                        <dc:title> ?title;
            } OFFSET $offset";
  if ($limit > 0) {
    $query .= " LIMIT $limit";
  }
  $tuque = islandora_get_tuque_connection();
  $results = $tuque->repository->ri->sparqlQuery($query);
  $locations = array();
  foreach ($results as $result) {
    if (isset($result['position']['value']) && preg_match('*,*', $result['position']['value'])) {
      $position = explode(',', $result['position']['value']);
      $location = new stdClass();
      $location->latitude = floatval($position[0]);
      $location->longitude = floatval($position[1]);
      $location->balloon_text = $result['title']['value'];
      $locations[] = $location;
    }
  }
  return $locations;
}
